#include <QApplication>
#include <QWidget>
#include <QSurfaceFormat>
#include <vector>
#include <cmath>

#include "PointTypes.h"
#include "MainWindow.h"
#include "PointCloudRepository.h"
#include "define.h"

#define MAX_GRID_SIZE 500
#define CELL_SIZE 0.5f

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
