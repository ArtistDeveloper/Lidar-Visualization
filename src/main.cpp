#include <QApplication>
#include <QWidget>
#include <QSurfaceFormat>
#include <vector>
#include <cmath>
#include <queue>

#include "PointTypes.h"
#include "MainWindow.h"
#include "PointCloudRepository.h"
#include "define.h"

#define MAX_GRID_SIZE 500
#define CELL_SIZE 0.5f

CELL_INFO grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
std::vector<int> gridIndices[MAX_GRID_SIZE][MAX_GRID_SIZE]; // 한 프레임 내에서 각 셀에서 가지고 있는 포인트 클라우드 데이터에 대한 인덱스

// Ground segmentation을 위한 rule-based 임계값
constexpr int MIN_POINTS_PER_CELL = 1;
constexpr float CELL_VAR_THRESH = 0.15f;      // seed 선정 용
constexpr float CELL_VAR_GROWTH = 0.25f;      // 확장 시 허용
constexpr float NEIGHBOR_MINZ_DIFF = 0.25f;   // 인접 ground 연속성 허용 높이차
constexpr float SEED_ABS_THRESH = 0.80f;      // globalMinZ 로부터 seed 허용 높이
constexpr float SEED_RADIUS = 6.0f;           // (m) 초기 seed 반경
constexpr float LOW_OBJECT_MAX_DELTA = 0.12f; // 지면이라고 봐 줄 수 있는 최대 허용 높이 차. 0.12m => 12cm까지는 거의 지면으로 인정

// iCellStatusFlag 값 정의 (POC)
enum : int
{
    CELL_UNKNOWN = 0,
    CELL_GROUND = 1,
    CELL_OBJECT = 2
};

struct SeedRejectStats
{
    int totalCandidates = 0;
    int rejectedEmpty = 0;
    int rejectedSpan = 0;
    int rejectedDz = 0;
    int rejectedRadius = 0;
};
SeedRejectStats stats;

// 8방향 이동
static const int OFFS[8][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

inline bool inBounds(int gx, int gy)
{
    return (gx >= 0 && gx < MAX_GRID_SIZE && gy >= 0 && gy < MAX_GRID_SIZE);
}

// 가장 낮은 z대신 하위 p-백분위수를 global baseline으로 사용
// 이거 대신 RNR을 사용해도 robust해질 수 있지 않을까?
float computeRobustGlobalMinZ(float percentile = 0.05f, int depth = 0)
{
    std::vector<float> mins;
    mins.reserve(10000);

    // 모든 그리드 셀에서 가장 낮은 z값을 탐색
    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
    {
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            const CELL_INFO &c = grid[gx][gy];
            if (c.NumOfPnt_CELL > 0)
                mins.push_back(c.fMinZ_GND);
        }
    }

    // 결과가 없다면, float의 MAX값 return
    if (mins.empty())
        return std::numeric_limits<float>::infinity();

    // 해당 백분위에 해당하는 값 ㅂ선택
    size_t k = static_cast<size_t>(std::clamp(percentile, 0.0f, 1.0f) * (mins.size() - 1));
    std::nth_element(mins.begin(), mins.begin() + k, mins.end());
    float val = mins[k];

    // 추정된 globalMinZ가 -8 아래이며 percentile이 0.15이면 값 재추출 시도
    if (val < -8.0f && percentile < 0.15f && depth < 3)
    {
        float repeatedP = std::min(percentile * 2.0f, 1.0f);
        return computeRobustGlobalMinZ(repeatedP, depth + 1);
    }
    return val;
}

void segmentGroundCells()
{
    float globalMinZ = computeRobustGlobalMinZ(0.05f);
    if (!std::isfinite(globalMinZ))
        return;

    // Seed (Ground) 선택
    std::vector<std::pair<int, int>> seedCells;
    seedCells.reserve(1024);

    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
    {
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            CELL_INFO &c = grid[gx][gy];

            if (c.NumOfPnt_CELL == 0)
                continue; // 완전 빈 셀은 통계 제외
            ++stats.totalCandidates;

            // 셀에 있는 최소 포인트 밀도 확인
            if (c.NumOfPnt_CELL < MIN_POINTS_PER_CELL)
            {
                ++stats.rejectedEmpty;
                continue;
            }

            // 셀 내부의 MaxZ - MinZ의 차이를 통해 얼마나 평탄한지 확인 (추후 variance나 stddev를 사용해보도록 변경)
            // minz는 지면인데, maxz는 객체이면 span이 크게나와서 객체 아래 바로 지면이 ground로 안잡힘
            float span = c.fMaxZ_GND - c.fMinZ_GND;
            if (span > CELL_VAR_THRESH)
            {
                ++stats.rejectedSpan;
                continue;
            }

            // 객체가 셀에 같이 들어오면 span이 커짐. 하지만 최저점은 여전히 지면일 수 있음 
            // cell 의 minz가 globalMinZ와 거의 같다면 해당 셀 안에는 지면이 포함되있을 것
            // float span = c.fMaxZ_GND - c.fMinZ_GND;
            // float dzGlobal = c.fMaxZ_GND - globalMinZ;
            // bool flatEnough = (span <= CELL_VAR_THRESH) || (dzGlobal < LOW_OBJECT_MAX_DELTA);
            // if (!flatEnough)
            // {
            //     ++stats.rejectedSpan;
            //     continue;
            // }

            // 셀의 최소 z값 - 프레임 전체에서 하위 p 백분위수로 잡은 지면 기준 높이
            // 허용되는 최대 높이 차를 통한 조건
            float dzGlobal = c.fMinZ_GND - globalMinZ;
            if (dzGlobal > SEED_ABS_THRESH)
            {
                ++stats.rejectedDz;
                continue;
            }

            // 센서 원점으로 부터 너무 먼 셀은 seed에서 제외
            float r = std::sqrt(c.centerX * c.centerX + c.centerY * c.centerY);
            if (r > SEED_RADIUS)
            {
                ++stats.rejectedRadius;
                continue;
            }

            c.iCellStatusFlag = CELL_GROUND;
            seedCells.emplace_back(gx, gy);
        }
    }

    // 선택된 sedd (ground)를 기반으로 BFS 확장
    std::queue<std::pair<int, int>> q;
    for (auto &p : seedCells)
        q.push(p);

    while (!q.empty())
    {
        auto [cx, cy] = q.front();
        q.pop();
        const CELL_INFO &curr = grid[cx][cy];

        for (auto &d : OFFS)
        {
            int nx = cx + d[0], ny = cy + d[1];
            if (!inBounds(nx, ny))
                continue;

            CELL_INFO &ncell = grid[nx][ny];
            if (ncell.NumOfPnt_CELL < MIN_POINTS_PER_CELL)
                continue;
            if (ncell.iCellStatusFlag == CELL_GROUND)
                continue; // 이미 ground

            float span = ncell.fMaxZ_GND - ncell.fMinZ_GND;
            if (span > CELL_VAR_GROWTH)
                continue;

            float dz = std::fabs(ncell.fMinZ_GND - curr.fMinZ_GND);
            if (dz > NEIGHBOR_MINZ_DIFF)
                continue;

            // 전역 바닥보다 너무 높이 띄어 있으면 제외 (과도한 계단/차량)
            if ((ncell.fMinZ_GND - globalMinZ) > 2.0f)
                continue;

            ncell.iCellStatusFlag = CELL_GROUND;
            q.emplace(nx, ny);
        }
    }

    // 4) 잔여 셀 분류
    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
    {
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            CELL_INFO &c = grid[gx][gy];
            if (c.NumOfPnt_CELL == 0)
                continue;

            if (c.iCellStatusFlag == CELL_GROUND)
                continue;

            // 선택적 승격(매우 낮고 평평)
            float span = c.fMaxZ_GND - c.fMinZ_GND;
            if (span < CELL_VAR_THRESH &&
                (c.fMinZ_GND - globalMinZ) < LOW_OBJECT_MAX_DELTA)
            {
                c.iCellStatusFlag = CELL_GROUND;
            }
            else
            {
                c.iCellStatusFlag = CELL_OBJECT;
            }
        }
    }

#define DEBUG_SEG
#ifdef DEBUG_SEG
    int cntCells = 0, cntGround = 0, cntObject = 0, cntSeed = 0;
    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
    {
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            auto &c = grid[gx][gy];
            if (c.NumOfPnt_CELL == 0)
                continue;
            ++cntCells;
            if (c.iCellStatusFlag == CELL_GROUND)
                ++cntGround;
            else if (c.iCellStatusFlag == CELL_OBJECT)
                ++cntObject;
        }
    }

    // total segmentation result
    qDebug() << "[SEG] cellsWithPoints=" << cntCells
             << " ground=" << cntGround
             << " object=" << cntObject;

    // seed segmentation debug
    qDebug() << "[SEED] totalCand=" << stats.totalCandidates
             << "seed count=" << seedCells.size()
             << "rejected empty=" << stats.rejectedEmpty
             << "rejected span=" << stats.rejectedSpan
             << "rejected dz=" << stats.rejectedDz
             << "rejected radius=" << stats.rejectedRadius
             << "\n";
#endif
    stats = {};
}

void buildGridForFrame(const std::vector<PointXYZI> &pts)
{
    const int GRID_HALF = MAX_GRID_SIZE / 2;

    // 매 프레임마다 데이터 초기화
    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            grid[gx][gy] = {};
            gridIndices[gx][gy].clear();
        }

    // 포인트 클라우드 데이터를 각 셀에 매핑
    for (unsigned int i = 0; i < pts.size(); ++i)
    {
        // Bird-view 시점으로 그리드 배열에 접근할 인덱스 gx, gy 추출
        const auto &p = pts[i];
        int gx = static_cast<int>(std::floor(p.x / CELL_SIZE)) + GRID_HALF;
        int gy = static_cast<int>(std::floor(p.y / CELL_SIZE)) + GRID_HALF;
        if (!inBounds(gx, gy))
            continue;

        CELL_INFO &c = grid[gx][gy];

        // 첫 방문 셀 초기화
        if (c.NumOfPnt_CELL == 0)
        {
            c.fMinZ_GND = c.fMaxZ_GND = p.z;
            // 인덱스를 데카르트 좌표계로 이동 후
            // 배열 인덱스 - 실제 번호에 대한 실제 위치 + 셀 폭의 절반 * CELL_SIZE
            // 먼저 0.5을 더하고 CELL_SIZE로 스케일링 해도 셀 중앙 이동과 같음
            c.centerX = (gx - GRID_HALF + 0.5f) * CELL_SIZE;
            c.centerY = (gy - GRID_HALF + 0.5f) * CELL_SIZE;
            c.iCellStatusFlag = CELL_UNKNOWN;
        }
        else
        {
            c.fMinZ_GND = std::min(c.fMinZ_GND, p.z);
            c.fMaxZ_GND = std::max(c.fMaxZ_GND, p.z);
        }
        ++c.NumOfPnt_CELL;
        gridIndices[gx][gy].push_back(static_cast<int>(i));
    }

    segmentGroundCells();
}

// Ground/Object intensity 결정 후 결과 out에 채워 반환
void buildGroundSegmentedAggregate(
    const std::vector<PointXYZI> &inPts,
    std::vector<PointXYZI> &out)
{
    buildGridForFrame(inPts); // segmentation 수행

    out.clear();
    out.reserve(inPts.size());

    for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
        for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
        {
            const auto &ids = gridIndices[gx][gy];
            if (ids.empty())
                continue;

            int flag = grid[gx][gy].iCellStatusFlag;

            // Ground=1.0f / Non-ground=0.0f (Unknown 포함)
            float f =
                (flag == CELL_GROUND) ? 1.0f : 0.0f;

            for (int id : ids)
            {
                PointXYZI p = inPts[id];
                p.intensity = f; // location=1 attribute 로 넘어감
                out.push_back(p);
            }
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
