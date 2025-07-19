#include <QApplication>
#include <QWidget>
#include <QSurfaceFormat>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <iostream>

#include "PointTypes.h"
#include "MainWindow.h"
#include "PointCloudRepository.h"
#include "define.h"

#define MAX_GRID_SIZE 500
#define CELL_SIZE 0.5f

static PWParams g_pw;
static std::vector<float> g_updateElev[4];
static std::vector<float> g_updateFlat[4];

CELL_INFO grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
std::vector<int> gridIndices[MAX_GRID_SIZE][MAX_GRID_SIZE]; // 셀 → pts 인덱스 리스트

void buildGridForFrame(const std::vector<PointXYZI> &pts)
{
    const int GRID_HALF = MAX_GRID_SIZE / 2;

    // 초기화: 메타데이터 & 인덱스 리스트 모두 비움
    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            grid[gx][gy] = {}; // 모든 필드 0 / nullptr
            gridIndices[gx][gy].clear();
        }

    // 셀 매핑
    for (unsigned int i = 0; i < pts.size(); ++i)
    {
        const auto &p = pts[i]; // p.x , p.y 는 KITTI 평면 좌표
        int gx = static_cast<int>(std::floor(p.x / CELL_SIZE)) + GRID_HALF;
        int gy = static_cast<int>(std::floor(p.y / CELL_SIZE)) + GRID_HALF;
        if (gx < 0 || gx >= MAX_GRID_SIZE || gy < 0 || gy >= MAX_GRID_SIZE)
        {
            qDebug() << "out of range";
            continue;
        }

        CELL_INFO &c = grid[gx][gy];

        // 첫 방문 셀: 초기화
        if (c.NumOfPnt_CELL == 0)
        {   
            c.fMinZ_GND = c.fMaxZ_GND = p.z;
            // 그리드 중심을 (0,0)으로 평행이동 후
            // 배열 인덱스 - 실 번호에 대한 실제 위치 + 셀 폭의 절반 * CELL_SIZE
            // 먼저 0.5를 더하고 CELL_SIZE로 스케일링 해도 셀 중앙 이동과 같음
            c.fX = (gx - GRID_HALF + 0.5f) * CELL_SIZE;
            c.fY = (gy - GRID_HALF + 0.5f) * CELL_SIZE;
        }
        else
        {
            c.fMinZ_GND = std::min(c.fMinZ_GND, p.z);
            c.fMaxZ_GND = std::max(c.fMaxZ_GND, p.z);
        }
        ++c.NumOfPnt_CELL;
        gridIndices[gx][gy].push_back(static_cast<int>(i)); // 현재 포인트가 pts벡터에서 몇 번째 인지 저장
    }
}

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setSwapInterval(0); // 수직 동기화 OFF
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    auto repo = std::make_shared<PointCloudRepository>(&app); // App이 삭제될 때 같이 메모리 해제. QObject에 종속하면 부모 오브젝트가 정리될때, 자식 오브젝트도 정리 가능
    MainWindow w(repo);
    w.show();

    return app.exec();
}


struct PlaneFit {
    Eigen::Vector3f normal {0,0,1};
    float d = 0.f;          // n·x + d = 0
    float flatness = 1e6f;  // 최소 고유값
};

static PlaneFit estimatePlane(const std::vector<Eigen::Vector3f>& pts)
{
    PlaneFit pf;
    if (pts.size() < 3) return pf;

    Eigen::MatrixX3f M(pts.size(),3);
    for (size_t i=0;i<pts.size();++i) M.row(i)=pts[i];
    Eigen::Vector3f mean = M.colwise().mean();
    Eigen::MatrixX3f centered = M.rowwise() - mean.transpose();
    Eigen::Matrix3f cov = (centered.adjoint()*centered) / std::max<int>((int)pts.size()-1,1);

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> sv(cov);
    pf.flatness = sv.eigenvalues()(0);
    pf.normal   = sv.eigenvectors().col(0);
    if (pf.normal.z() < 0) pf.normal = -pf.normal;
    pf.d = -pf.normal.dot(mean);
    return pf;
}

static float pointPlaneDist(const Eigen::Vector3f& p, const PlaneFit& pf)
{
    return pf.normal.dot(p) + pf.d;
}

// LPR 기반 seed 추출
static void extractSeeds(const std::vector<Eigen::Vector3f>& sortedZ,
                         std::vector<Eigen::Vector3f>& seeds,
                         int num_lpr, float th_seed)
{
    seeds.clear();
    if (sortedZ.empty()) return;

    double sum=0; int cnt=0;
    for(size_t i=0;i<sortedZ.size() && cnt<num_lpr;++i){
        sum += sortedZ[i].z(); ++cnt;
    }
    float lpr = cnt? (float)(sum/cnt) : 0.f;

    for(auto& p: sortedZ){
        if (p.z() < lpr + th_seed) seeds.push_back(p);
    }
}

static void updateAdaptiveThresholds()
{
    for(int r=0;r<g_pw.num_rings_of_interest;++r){
        auto& EV = g_updateElev[r];
        auto& FV = g_updateFlat[r];

        if(EV.size()>=5){
            float mean = std::accumulate(EV.begin(),EV.end(),0.f)/EV.size();
            float var=0; for(float v:EV) var+=(v-mean)*(v-mean);
            float sd = (EV.size()>1)? std::sqrt(var/(EV.size()-1)) : 0.f;
            if(r==0){
                g_pw.elevation_thr[r] = mean + 3*sd;
                g_pw.sensor_height = -mean; // 원 코드처럼 센서 높이 추정 보정
            } else {
                g_pw.elevation_thr[r] = mean + 2*sd;
            }
            if(EV.size()>g_pw.max_store) EV.erase(EV.begin(), EV.begin()+(EV.size()-g_pw.max_store));
        }
        if(FV.size()>=5){
            float mean = std::accumulate(FV.begin(),FV.end(),0.f)/FV.size();
            float var=0; for(float v:FV) var+=(v-mean)*(v-mean);
            float sd = (FV.size()>1)? std::sqrt(var/(FV.size()-1)) : 0.f;
            g_pw.flatness_thr[r] = mean + sd;
            if(FV.size()>g_pw.max_store) FV.erase(FV.begin(), FV.begin()+(FV.size()-g_pw.max_store));
        }
    }
}

// ring 폭 (m) – TGR/near 구간 경계
static float g_ringWidth = 5.f;

void classifyGround_RectGrid(const std::vector<PointXYZI>& pts,
                                    std::vector<int>& groundIdx,
                                    std::vector<int>& nongroundIdx)
{
    groundIdx.clear(); nongroundIdx.clear();

    // 1) 패치 목록 생성
    struct Patch { float r; CELL_INFO_t* c; const std::vector<int>* ids; };
    std::vector<Patch> patches;
    patches.reserve(20000);
    for(int gx=0; gx<MAX_GRID_SIZE; ++gx)
        for(int gy=0; gy<MAX_GRID_SIZE; ++gy){
            auto& ids = gridIndices[gx][gy];
            if(ids.empty()) continue;
            CELL_INFO_t& c = grid[gx][gy];
            float r = std::hypot(c.fX, c.fY);
            patches.push_back({r, &c, &ids});
        }
    std::sort(patches.begin(), patches.end(),
              [](auto&a,auto&b){ return a.r < b.r; });

    // TGR 준비
    size_t ringIdx = 0;
    float nextRingBoundary = g_ringWidth;
    std::vector<float> ringFlatness;
    struct Cand { size_t ring; float flat; std::vector<int> ids; };
    std::vector<Cand> candidates;

    auto flushTGR = [&](){
        if(!g_pw.enable_TGR || candidates.empty()) return;
        // 간단한 revert: 평균+1.5σ 기준
        float mu = std::accumulate(ringFlatness.begin(), ringFlatness.end(), 0.f) /
                   std::max<size_t>(1, ringFlatness.size());
        float var=0.f; for(float v:ringFlatness) var+=(v-mu)*(v-mu);
        float sd = ringFlatness.size()>1? std::sqrt(var/(ringFlatness.size()-1)):0.f;
        float thresh = mu + 1.5f*sd;
        for(auto& c: candidates){
            bool revert = (c.flat < thresh);
            auto& dst = revert ? groundIdx : nongroundIdx;
            dst.insert(dst.end(), c.ids.begin(), c.ids.end());
        }
        candidates.clear();
        ringFlatness.clear();
    };

    // 2) 패치 반복
    for(const auto& patch: patches)
    {
        // ring 경계 넘으면 TGR 처리
        if(patch.r > nextRingBoundary){
            flushTGR();
            ++ringIdx;
            nextRingBoundary += g_ringWidth;
        }

        const auto& ids = *patch.ids;
        if((int)ids.size() < g_pw.num_min_pts){
            // 너무 작은 패치 → 비지상으로 둔다 (또는 그대로 pass)
            nongroundIdx.insert(nongroundIdx.end(), ids.begin(), ids.end());
            continue;
        }

        // 2-1) patch 포인트 수집 + z 정렬
        std::vector<Eigen::Vector3f> pvec; pvec.reserve(ids.size());
        for(int id: ids) pvec.emplace_back(pts[id].x, pts[id].y, pts[id].z);
        std::sort(pvec.begin(), pvec.end(),
                  [](auto&a,auto&b){ return a.z() < b.z(); });

        // ------- R‑VPF (간단 버전) -------
        std::vector<Eigen::Vector3f> src = pvec;
        if(g_pw.enable_RVPF){
            for(int iter=0; iter<g_pw.num_iter; ++iter){
                std::vector<Eigen::Vector3f> seeds;
                extractSeeds(src, seeds, g_pw.num_lpr, g_pw.th_seeds_v);
                if(seeds.size() < 3) break;
                PlaneFit plane = estimatePlane(seeds);
                if(iter==0 && plane.normal.z() < g_pw.uprightness_thr){
                    // vertical 로 보고 “plane 부근”을 제외
                    std::vector<Eigen::Vector3f> filtered;
                    filtered.reserve(src.size());
                    for(auto& p: src){
                        float dist = std::fabs(pointPlaneDist(p, plane));
                        if(dist > g_pw.th_dist_v) filtered.push_back(p); // plane 근처는 제거
                    }
                    if(filtered.size() >= (size_t)g_pw.num_min_pts) src.swap(filtered);
                    else break;
                } else break;
            }
        }

        // ------- R‑GPF -------
        std::vector<Eigen::Vector3f> seeds;
        extractSeeds(src, seeds, g_pw.num_lpr, g_pw.th_seeds);
        if(seeds.size() < 3){
            nongroundIdx.insert(nongroundIdx.end(), ids.begin(), ids.end());
            continue;
        }
        PlaneFit plane = estimatePlane(seeds);

        std::vector<int> patchGround; patchGround.reserve(src.size());
        std::vector<int> patchNG;     patchNG.reserve(src.size());

        for(int iter=0; iter<g_pw.num_iter; ++iter){
            patchGround.clear(); patchNG.clear();
            // 거리 판별
            for(size_t i=0;i<src.size();++i){
                float dist = pointPlaneDist(src[i], plane);
                bool isG = dist < g_pw.th_dist;
                if(iter < g_pw.num_iter-1){
                    if(isG) patchGround.push_back((int)i);
                } else {
                    int originalId = ids[i];
                    (isG? patchGround:patchNG).push_back(originalId);
                }
            }
            if(iter < g_pw.num_iter-1){
                std::vector<Eigen::Vector3f> newSeeds;
                newSeeds.reserve(patchGround.size());
                for(int idx: patchGround) newSeeds.push_back(src[idx]);
                if(newSeeds.size()<3) break;
                plane = estimatePlane(newSeeds);
            }
        }

        // ------- A‑GLE 조건 -------
        bool is_near         = ringIdx < (size_t)g_pw.num_rings_of_interest;
        bool is_upright      = plane.normal.z() > g_pw.uprightness_thr;
        bool is_not_elevated = (patch.c->fMinZ_GND <
                               g_pw.elevation_thr[std::min<int>((int)ringIdx, g_pw.num_rings_of_interest-1)]);
        bool is_flat         = (plane.flatness <
                               g_pw.flatness_thr[std::min<int>((int)ringIdx, g_pw.num_rings_of_interest-1)]);

        // heading 체크 (센서 원점→평균 위치 벡터와 법선)
        Eigen::Vector3f meanCell(patch.c->fX, patch.c->fY, (patch.c->fMinZ_GND + patch.c->fMaxZ_GND)*0.5f);
        float heading = plane.normal.dot(meanCell); // sign

        bool heading_ok = (heading < 0.f) || !is_near; // 원 코드 heading<0 조건 단순화

        if(!is_upright){
            nongroundIdx.insert(nongroundIdx.end(), patchGround.begin(), patchGround.end());
            nongroundIdx.insert(nongroundIdx.end(), patchNG.begin(), patchNG.end());
            continue;
        }
        if(!is_near){
            groundIdx.insert(groundIdx.end(), patchGround.begin(), patchGround.end());
            nongroundIdx.insert(nongroundIdx.end(), patchNG.begin(), patchNG.end());
            continue;
        }
        if(!heading_ok){
            nongroundIdx.insert(nongroundIdx.end(), patchGround.begin(), patchGround.end());
            nongroundIdx.insert(nongroundIdx.end(), patchNG.begin(), patchNG.end());
            continue;
        }
        if(is_not_elevated || is_flat){
            groundIdx.insert(groundIdx.end(), patchGround.begin(), patchGround.end());
            nongroundIdx.insert(nongroundIdx.end(), patchNG.begin(), patchNG.end());

            // 통계 (near ring만)
            if(is_near){
                g_updateElev[ringIdx].push_back(patch.c->fMinZ_GND);
                g_updateFlat[ringIdx].push_back(plane.flatness);
                ringFlatness.push_back(plane.flatness);
            }
        } else {
            // 후보 → TGR 로 defer
            candidates.push_back({ringIdx, plane.flatness, patchGround});
            nongroundIdx.insert(nongroundIdx.end(), patchNG.begin(), patchNG.end());
        }
    }

    // 마지막 ring flush
    flushTGR();

    // Adaptive thresholds update
    updateAdaptiveThresholds();
}
