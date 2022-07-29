#ifndef CPUINFER_H
#define CPUINFER_H
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <inference_engine.hpp>
#include <chrono>

/**
 * @brief Define names based depends on Unicode path support
 */
#define tcout                  std::cout
#define file_name_t            std::string
#define imread_t               cv::imread
#define NMS_THRESH 0.45
#define BBOX_CONF_THRESH 0.3

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

typedef struct GridAndStride
{
    int grid0;
    int grid1;
    int stride;
} GridAndStride;

class CPUInfer
{
public:
    CPUInfer();
    ~CPUInfer();

    cv::Mat static_resize(cv::Mat& img);

    void blobFromImage(cv::Mat& img, InferenceEngine::Blob::Ptr& blob);

    static void generate_grids_and_stride(const int target_w,
                                          const int target_h,
                                          std::vector<int>& strides,
                                          std::vector<GridAndStride>& grid_strides);

    static void generate_yolox_proposals(std::vector<GridAndStride> grid_strides,
                                         const float* feat_ptr,
                                         float prob_threshold,
                                         std::vector<Object>& objects);

    static inline float intersection_area(const Object& a, const Object& b);

    static void qsort_descent_inplace(std::vector<Object>& faceobjects, int left, int right);

    static void qsort_descent_inplace(std::vector<Object>& objects);

    static void nms_sorted_bboxes(const std::vector<Object>& faceobjects, std::vector<int>& picked, float nms_threshold);

    static void decode_outputs(const float* prob, std::vector<Object>& objects, float scale, const int img_w, const int img_h);

    static cv::Mat draw_objects(const cv::Mat& bgr, const std::vector<Object>& objects, int inferTime);

    int initEngine(std::string networkFilePath, std::string inferDeviceName);

    cv::Mat startInfer(cv::Mat image);
private:
    std::string input_name;
    std::string output_name;
    InferenceEngine::InferRequest infer_request;
};

// initialize some data here..
// be careful with the 'static const' or 'const static' varibles
// best way is to put them out of the class and initialize them
static const int INPUT_W = 416;
static const int INPUT_H = 416;
static const int NUM_CLASSES = 80; // COCO has 80 classes. Modify this value on your own dataset.

// NO IDEA why we need so many colors...
const float color_list[80][3] =
{
    {0.000, 0.447, 0.741},
    {0.850, 0.325, 0.098},
    {0.929, 0.694, 0.125},
    {0.494, 0.184, 0.556},
    {0.466, 0.674, 0.188},
    {0.301, 0.745, 0.933},
    {0.635, 0.078, 0.184},
    {0.300, 0.300, 0.300},
    {0.600, 0.600, 0.600},
    {1.000, 0.000, 0.000},
    {1.000, 0.500, 0.000},
    {0.749, 0.749, 0.000},
    {0.000, 1.000, 0.000},
    {0.000, 0.000, 1.000},
    {0.667, 0.000, 1.000},
    {0.333, 0.333, 0.000},
    {0.333, 0.667, 0.000},
    {0.333, 1.000, 0.000},
    {0.667, 0.333, 0.000},
    {0.667, 0.667, 0.000},
    {0.667, 1.000, 0.000},
    {1.000, 0.333, 0.000},
    {1.000, 0.667, 0.000},
    {1.000, 1.000, 0.000},
    {0.000, 0.333, 0.500},
    {0.000, 0.667, 0.500},
    {0.000, 1.000, 0.500},
    {0.333, 0.000, 0.500},
    {0.333, 0.333, 0.500},
    {0.333, 0.667, 0.500},
    {0.333, 1.000, 0.500},
    {0.667, 0.000, 0.500},
    {0.667, 0.333, 0.500},
    {0.667, 0.667, 0.500},
    {0.667, 1.000, 0.500},
    {1.000, 0.000, 0.500},
    {1.000, 0.333, 0.500},
    {1.000, 0.667, 0.500},
    {1.000, 1.000, 0.500},
    {0.000, 0.333, 1.000},
    {0.000, 0.667, 1.000},
    {0.000, 1.000, 1.000},
    {0.333, 0.000, 1.000},
    {0.333, 0.333, 1.000},
    {0.333, 0.667, 1.000},
    {0.333, 1.000, 1.000},
    {0.667, 0.000, 1.000},
    {0.667, 0.333, 1.000},
    {0.667, 0.667, 1.000},
    {0.667, 1.000, 1.000},
    {1.000, 0.000, 1.000},
    {1.000, 0.333, 1.000},
    {1.000, 0.667, 1.000},
    {0.333, 0.000, 0.000},
    {0.500, 0.000, 0.000},
    {0.667, 0.000, 0.000},
    {0.833, 0.000, 0.000},
    {1.000, 0.000, 0.000},
    {0.000, 0.167, 0.000},
    {0.000, 0.333, 0.000},
    {0.000, 0.500, 0.000},
    {0.000, 0.667, 0.000},
    {0.000, 0.833, 0.000},
    {0.000, 1.000, 0.000},
    {0.000, 0.000, 0.167},
    {0.000, 0.000, 0.333},
    {0.000, 0.000, 0.500},
    {0.000, 0.000, 0.667},
    {0.000, 0.000, 0.833},
    {0.000, 0.000, 1.000},
    {0.000, 0.000, 0.000},
    {0.143, 0.143, 0.143},
    {0.286, 0.286, 0.286},
    {0.429, 0.429, 0.429},
    {0.571, 0.571, 0.571},
    {0.714, 0.714, 0.714},
    {0.857, 0.857, 0.857},
    {0.000, 0.447, 0.741},
    {0.314, 0.717, 0.741},
    {0.50, 0.5, 0}
};

static const char* class_names[] = {
    "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
    "hair drier", "toothbrush"
};

#endif // CPUINFER_H
