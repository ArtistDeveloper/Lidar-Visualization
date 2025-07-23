#define MAX_NUM_POINT 150000
#define MAX_NUM_POINT_PER_OBJECT 40000
#define MAX_NUM_OBJECT_CLUSTERING 3000


typedef struct CELL_INFO
{
   float fMinZ_GND;
   float fMaxZ_GND;
   float centerX;
   float centerY;
   int iCellStatusFlag;
   int NumOfPnt_CELL;
} CELL_INFO_t;


typedef struct POINT_INFO
{
   float fX_m;
   float fY_m;
   float fZ_m;
   float fIntensity;
   float fTheta_deg;
   int iLayer;
   int iCloseFlag;
   int iPntStatusFlag; //0:Unvalid, 1:Ground, 2:Object, 3:No Use
   int iRoadInfoFlag; //0:No roadInfo, 1:Lane, 3:Curb
} POINT_INFO_t;


typedef struct tLIDAR_POINT
{
   int iInitFlag;
   int iNumOfPnt;
   int iAzimuth;
   POINT_INFO_t astPntInfo[MAX_NUM_POINT]; 
} LIDAR_POINT_t;


typedef struct SEGMENT_INFO_t
{
   float fMaxX;
   float fMinX;
   float fMaxY;
   float fMinY;
   float fMaxZ;
   float fMinZ;
} SEGMENT_INFO;


typedef struct EACH_SEGMENT_t
{
   int iNumOfPnt; // 각 object마다의 point 갯수
   int aiPntId[MAX_NUM_POINT_PER_OBJECT]; // point마다 ID
   SEGMENT_INFO stSegInfo;
} EACH_SEGMENT;


typedef struct EACH_CONVEX_t
{
   int iNumOfPnt; // 각 object마다의 point 갯수
   float afX[MAX_NUM_POINT_PER_OBJECT]; // convex_hull_coord_fX[point마다 ID]
   float afY[MAX_NUM_POINT_PER_OBJECT]; // convex_hull_coord_fY[point마다 ID]
   double minfZ;
   double maxfZ;
   double fX1; // 박스 X, Y좌표
   double fX2;
   double fX3;
   double fX4;
   double fY1;
   double fY2;
   double fY3;
   double fY4;
   double Width;
   double Length;
   double Height;
   
} EACH_CONVEX;


typedef struct SEGMENT_t
{
   int iNumOfSeg; // object 갯수
   EACH_SEGMENT astEachSeg[MAX_NUM_OBJECT_CLUSTERING];
   EACH_CONVEX astEachConvex[MAX_NUM_OBJECT_CLUSTERING];
   double fX1; // 박스 x, y 좌표
   double fX2;
   double fX3;
   double fX4;
   double fY1;
   double fY2;
   double fY3;
   double fY4;
   double Width;
   double Length;
   double Height;
   // EACH_BBOX_FX
   // EACH_BBOX_FY

} SEGMENT;


typedef struct tGL_3F
{
   float fX;
   float fY;
   float fZ;
} GL_3F_t;