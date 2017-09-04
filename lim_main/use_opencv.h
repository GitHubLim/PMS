#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videostab.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/shape/shape.hpp"
#include "opencv2/superres.hpp"
#include "opencv2/stitching.hpp"
#include "opencv2/tracking.hpp"

#ifdef _DEBUG
	
	#pragma comment(lib,"opencv_tracking320d.lib")
#else
	//#pragma comment(lib,"opencv_core320.lib")
	#pragma comment(lib,"opencv_highgui320.lib")
	#pragma comment(lib,"opencv_imgproc320.lib")
	#pragma comment(lib,"opencv_imgcodecs320.lib")
	#pragma comment(lib,"opencv_objdetect320.lib")
	#pragma comment(lib,"opencv_video320.lib")
	#pragma comment(lib,"opencv_videostab320.lib")
	#pragma comment(lib,"opencv_videoio320.lib")
	#pragma comment(lib,"opencv_features2d320.lib")
	#pragma comment(lib,"opencv_flann320.lib")
	#pragma comment(lib,"opencv_photo320.lib")
	#pragma comment(lib,"opencv_calib3d320.lib")
	#pragma comment(lib,"opencv_shape320.lib")
	#pragma comment(lib,"opencv_ml320.lib")
	#pragma comment(lib,"opencv_superres320.lib")
	#pragma comment(lib,"opencv_ts320.lib")
	#pragma comment(lib,"opencv_stitching320.lib")
	#pragma comment(lib,"opencv_hal320.lib")
	#pragma comment(lib,"opencv_tracking320.lib")
#endif

