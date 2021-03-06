// ----------------------- OpenPose C++ API Tutorial - Example 4 - Body from images ----------------------
// It reads images, process them, and display them with the pose (and optionally hand and face) keypoints. In addition,
// it includes all the OpenPose configuration flags (enable/disable hand, face, output saving, etc.).

// Third-party dependencies
#include <opencv2/opencv.hpp>
// Command-line user interface
#define OPENPOSE_FLAGS_DISABLE_PRODUCER
#define OPENPOSE_FLAGS_DISABLE_DISPLAY
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;
Json::Reader reader;
Json::Value obj;

// Custom OpenPose flags
// Producer
DEFINE_string(image_dir, "examples/media1/",
    "Process a directory of images. Read all standard formats (jpg, png, bmp, etc.).");
// Display
DEFINE_bool(no_display, false,
    "Enable to disable the visual display.");

// This worker will just read and return all the jpg files in a directory
bool display(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr)
{
    try
    {
        // User's displaying/saving/other processing here
            // datum.cvOutputData: rendered frame with pose or heatmaps
            // datum.poseKeypoints: Array<float> with the estimated pose
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {
            // Display image and sleeps at least 1 ms (it usually sleeps ~5-10 msec to display the image)
            const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
            cv::imshow(OPEN_POSE_NAME_AND_VERSION + " - Tutorial C++ API", cvMat);
        }
        else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
        const auto key = (char)cv::waitKey(1);
        return (key == 27);
    }
    catch (const std::exception & e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        return true;
    }
}

void printKeypoints(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr)
{
    try
    {

        // Example: How to use the pose keypoints
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {
            //op::opLog("Body keypoints: " + datumsPtr->at(0)->poseKeypoints.toString(), op::Priority::High);
            op::opLog("\nKeypoints:", op::Priority::High);
            const auto& poseKeypoints = datumsPtr->at(0)->poseKeypoints;
            op::opLog("Person pose keypoints:", op::Priority::High);
            for (auto person = 0; person < poseKeypoints.getSize(0); person++)
            {
                op::opLog("Person " + std::to_string(person), op::Priority::High);
                //for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1); bodyPart++)
                //{
                   // op::opLog("bodyPart" + std::to_string(bodyPart), op::Priority::High);

                auto y2 = poseKeypoints[{person, 2, 1}];
                auto y3 = poseKeypoints[{person, 3, 1}];
                auto y4 = poseKeypoints[{person, 4, 1}];
                auto y5 = poseKeypoints[{person, 5, 1}];
                auto y6 = poseKeypoints[{person, 6, 1}];
                auto y7 = poseKeypoints[{person, 7, 1}];

                op::opLog("y2 = " + std::to_string(y2) + " y3 = " + std::to_string(y3) + " y4 = " + std::to_string(y4), op::Priority::High);
                op::opLog("y5 = " + std::to_string(y5) + " y6 = " + std::to_string(y6) + " y7 = " + std::to_string(y7), op::Priority::High);
                if (y2 - y3 >= 0 && y3 - y4 > 0 || y5 - y6 >= 0 && y6 - y7 > 0)
                {
                    //opLog("right hands up" , op::Priority::High);
                    if (y2 - y3 >= 0 && y3 - y4 > 0 && y5 - y6 >= 0 && y6 - y7 > 0)
                    {
                        opLog("both hands up", op::Priority::High);
                        auto r = cpr::Put(cpr::Url{ "https://pose-detection-8bab0.firebaseio.com/pose-detection.json" },
                            cpr::Body{ R"({"Id":1, "Action":"Both hands up"})" },
                            cpr::Header{ { "Content-Type", "json" } });
                        string action = "1";
                        auto x = cpr::Get(cpr::Url{ "https://pose-detection-1-6227a.firebaseio.com/espremote/.json" });
                        auto y = x.text;
                        reader.parse(y, obj);
                        auto espdata = obj["value"].asString();
                        cout << espdata << endl;
                        if (espdata == action)
                        {
                            cout << "HIT" << endl;
                        }
                        else
                        {
                            cout << "MISS" << endl;
                        }
                        break;
                    }
                    if (y5 - y6 >= 0 && y6 - y7 > 0)
                    {
                        opLog("left hands up", op::Priority::High);
                        auto r = cpr::Put(cpr::Url{ "https://pose-detection-8bab0.firebaseio.com/pose-detection.json" },
                            cpr::Body{ R"({"Id":2, "Action":"left hand up"})" },
                            cpr::Header{ { "Content-Type", "json" } });
                        string action = "2";
                        auto x = cpr::Get(cpr::Url{ "https://pose-detection-1-6227a.firebaseio.com/espremote/.json" });
                        auto y = x.text;
                        reader.parse(y, obj);
                        auto espdata = obj["value"].asString();
                        cout << espdata << endl;
                        if (espdata == action)
                        {
                            cout << "HIT" << endl;
                        }
                        else
                        {
                            cout << "MISS" << endl;
                        }
                    }
                    if (y2 - y3 >= 0 && y3 - y4 > 0)
                    {
                        opLog("right hands up", op::Priority::High);
                        auto r = cpr::Put(cpr::Url{ "https://pose-detection-8bab0.firebaseio.com/pose-detection.json" },
                            cpr::Body{ R"({"Id":3, "Name":"right hand up"})" },
                            cpr::Header{ { "Content-Type", "json" } });
                        string action = "3";
                        auto x = cpr::Get(cpr::Url{ "https://pose-detection-1-6227a.firebaseio.com/espremote/.json" });
                        auto y = x.text;
                        reader.parse(y, obj);
                        auto espdata = obj["value"].asString();
                        cout << espdata << endl;
                        if (espdata == action)
                        {
                            cout << "HIT" << endl;
                        }
                        else
                        {
                            cout << "MISS" << endl;
                        }
                    }
                    
                }
                else {
                    opLog("both hands down", op::Priority::High);
                    auto r = cpr::Put(cpr::Url{ "https://pose-detection-8bab0.firebaseio.com/pose-detection.json" },
                        cpr::Body{ R"({"Id":4, "Action":"both hands down"})" },
                        cpr::Header{ { "Content-Type", "json" } });
                    string action = "4";
                    auto x = cpr::Get(cpr::Url{ "https://pose-detection-1-6227a.firebaseio.com/espremote/.json" });
                    auto y = x.text;
                    reader.parse(y, obj);
                    auto espdata = obj["value"].asString();
                    cout << espdata << endl;
                    if (espdata == action)
                    {
                        cout << "HIT" << endl;
                    }
                    else
                    {
                        cout << "MISS" << endl;
                    }
                }

                //op::opLog(poseKeypoints, op::Priority::High);


                op::opLog(" ", op::Priority::High);
                /*std::cout << "Action: Retrieve the product with id = 1" << std::endl;
                auto x = cpr::Get(cpr::Url{ "https://pose-detection-1-6227a.firebaseio.com/espremote/.json" });
                cout << x.text << endl;
                auto y = x.text;
                reader.parse(y, obj);
         
                cout <<  obj["value"].asString();
                cout << std::endl;
                auto z = obj["value"].asString();
                cout << z << endl;*/
            }





        }
        else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);

    }
    catch (const std::exception & e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

void configureWrapper(op::Wrapper& opWrapper)
{
    try
    {
        // Configuring OpenPose

        // logging_level
        op::checkBool(
            0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.",
            __LINE__, __FUNCTION__, __FILE__);
        op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
        op::Profiler::setDefaultX(FLAGS_profile_speed);

        // Applying user defined configuration - GFlags to program variables
        // outputSize
        const auto outputSize = op::flagsToPoint(op::String(FLAGS_output_resolution), "-1x-1");
        // netInputSize
        const auto netInputSize = op::flagsToPoint(op::String(FLAGS_net_resolution), "-1x368");
        // poseMode
        const auto poseMode = op::flagsToPoseMode(FLAGS_body);
        // poseModel
        const auto poseModel = op::flagsToPoseModel(op::String(FLAGS_model_pose));
        // JSON saving
        if (!FLAGS_write_keypoint.empty())
            op::opLog(
                "Flag `write_keypoint` is deprecated and will eventually be removed. Please, use `write_json`"
                " instead.", op::Priority::Max);
        // keypointScaleMode
        const auto keypointScaleMode = op::flagsToScaleMode(FLAGS_keypoint_scale);
        // heatmaps to add
        const auto heatMapTypes = op::flagsToHeatMaps(FLAGS_heatmaps_add_parts, FLAGS_heatmaps_add_bkg,
            FLAGS_heatmaps_add_PAFs);
        const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(FLAGS_heatmaps_scale);
        // >1 camera view?
        const auto multipleView = (FLAGS_3d || FLAGS_3d_views > 1);
        // Enabling Google Logging
        const bool enableGoogleLogging = true;

        // Pose configuration (use WrapperStructPose{} for default and recommended configuration)
        const op::WrapperStructPose wrapperStructPose{
            poseMode, netInputSize, outputSize, keypointScaleMode, FLAGS_num_gpu, FLAGS_num_gpu_start,
            FLAGS_scale_number, (float)FLAGS_scale_gap, op::flagsToRenderMode(FLAGS_render_pose, multipleView),
            poseModel, !FLAGS_disable_blending, (float)FLAGS_alpha_pose, (float)FLAGS_alpha_heatmap,
            FLAGS_part_to_show, op::String(FLAGS_model_folder), heatMapTypes, heatMapScaleMode, FLAGS_part_candidates,
            (float)FLAGS_render_threshold, FLAGS_number_people_max, FLAGS_maximize_positives, FLAGS_fps_max,
            op::String(FLAGS_prototxt_path), op::String(FLAGS_caffemodel_path),
            (float)FLAGS_upsampling_ratio, enableGoogleLogging };
        opWrapper.configure(wrapperStructPose);
        // Extra functionality configuration (use op::WrapperStructExtra{} to disable it)
        const op::WrapperStructExtra wrapperStructExtra{
            FLAGS_3d, FLAGS_3d_min_views, FLAGS_identification, FLAGS_tracking, FLAGS_ik_threads };
        opWrapper.configure(wrapperStructExtra);
        // Output (comment or use default argument to disable any output)
        const op::WrapperStructOutput wrapperStructOutput{
            FLAGS_cli_verbose, op::String(FLAGS_write_keypoint), op::stringToDataFormat(FLAGS_write_keypoint_format),
            op::String(FLAGS_write_json), op::String(FLAGS_write_coco_json), FLAGS_write_coco_json_variants,
            FLAGS_write_coco_json_variant, op::String(FLAGS_write_images), op::String(FLAGS_write_images_format),
            op::String(FLAGS_write_video), FLAGS_write_video_fps, FLAGS_write_video_with_audio,
            op::String(FLAGS_write_heatmaps), op::String(FLAGS_write_heatmaps_format), op::String(FLAGS_write_video_3d),
            op::String(FLAGS_write_video_adam), op::String(FLAGS_write_bvh), op::String(FLAGS_udp_host),
            op::String(FLAGS_udp_port) };
        opWrapper.configure(wrapperStructOutput);
        // No GUI. Equivalent to: opWrapper.configure(op::WrapperStructGui{});
        // Set to single-thread (for sequential processing and/or debugging and/or reducing latency)
        if (FLAGS_disable_multi_thread)
            opWrapper.disableMultiThreading();
    }
    catch (const std::exception & e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

int tutorialApiCpp()
{
    try
    {
        op::opLog("Starting OpenPose demo...", op::Priority::High);
        const auto opTimer = op::getTimerInit();

        // Configuring OpenPose
        op::opLog("Configuring OpenPose...", op::Priority::High);
        op::Wrapper opWrapper{ op::ThreadManagerMode::Asynchronous };
        configureWrapper(opWrapper);

        // Starting OpenPose
        op::opLog("Starting thread(s)...", op::Priority::High);
        opWrapper.start();

        // Read frames on directory
        const auto imagePaths = op::getFilesOnDirectory(FLAGS_image_dir, op::Extensions::Images);

        // Process and display images
        for (const auto& imagePath : imagePaths)
        {
            const cv::Mat cvImageToProcess = cv::imread(imagePath);
            const op::Matrix imageToProcess = OP_CV2OPCONSTMAT(cvImageToProcess);
            auto datumProcessed = opWrapper.emplaceAndPop(imageToProcess);
            if (datumProcessed != nullptr)
            {
                printKeypoints(datumProcessed);
                if (!FLAGS_no_display)
                {
                    const auto userWantsToExit = display(datumProcessed);
                    if (userWantsToExit)
                    {
                        op::opLog("User pressed Esc to exit demo.", op::Priority::High);
                        break;
                    }
                }
            }
            else
                op::opLog("Image " + imagePath + " could not be processed.", op::Priority::High);
        }

        // Measuring total time
        op::printTime(opTimer, "OpenPose demo successfully finished. Total time: ", " seconds.", op::Priority::High);

        // Return
        return 0;
    }
    catch (const std::exception&)
    {
        return -1;
    }
}

int main(int argc, char* argv[])
{
    // Parsing command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Running tutorialApiCpp
    return tutorialApiCpp();
}
