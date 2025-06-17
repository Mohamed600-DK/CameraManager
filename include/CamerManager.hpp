#ifndef CLIENT_CAMERA_MANAGER_HPP
#define CLIENT_CAMERA_MANAGER_HPP

#include <memory>
#include <cstdint>

namespace cv
{
    class Mat;
}

namespace CameraManager
{
    /**
     * @brief Enum to represent the status of a function.
     *
     * This enum defines the possible return values of functions indicating whether
     * the function completed successfully or encountered an error.
     */
    typedef enum fun_status : uint8_t
    {
        FUN_ERROR = 0, ///< Function encountered an ERROR
        FUN_OK,        ///< Function executed successfully
    } fun_status_t;

    /**
     * @brief Enum to represent different image resolution options.
     *
     * This enum defines the available resolutions for the camera feed or image.
     */
    typedef enum image_resolution : uint8_t
    {
        NULL_RES = 0,
        SD, ///< Standard Definition (640x480)
        HD, ///< High Definition (1280x720)
        FHD ///< Full High Definition (1920x1080)
    } image_resolution_t;

    /**
     * @brief Struct to hold image properties.
     *
     * This struct contains the various properties related to the camera image such as:
     * - FPS (frames per second)
     * - Brightness
     * - Contrast
     * - Saturation
     * - Resolution
     */
    typedef struct image_properties
    {
        double Image_FPS;                          ///< Frames per second for the camera feed
        double Image_Brightness;                   ///< Brightness level of the image
        double Image_Contrast;                     ///< Contrast level of the image
        double Image_Saturation;                   ///< Saturation level of the image
        image_resolution_t Image_Frame_Resolution; ///< Resolution setting for the image (SD, HD, FHD)
    } image_properties_t;

    typedef enum camera_status : uint8_t
    {
        CAMERA_CLOSE,
        CAMERA_OPEN,
    } camera_status_t;

    typedef enum flag_status : uint8_t
    {
        UNSET = 0,
        SET = 1

    } flag_status_t;

    /**
     * @class CAMERA_MANAGER
     * @brief A class to manage camera operations such as capturing images, displaying video,
     *        and retrieving captured images in various formats.
     */
    class CAMERA_MANAGER
    {
    private:
        class ClassAttributes;
        std::unique_ptr<ClassAttributes> class_attribute;

    private:
        int32_t FindCameraIndexByModelName(const std::string &targetModel);

    public:
        /**
         * @brief Constructor to initialize the camera manager with a specified camera index.
         * @param camera_index The index of the camera to be used (e.g., 0 for default camera).
         */
        CAMERA_MANAGER(uint8_t camera_index);

        CAMERA_MANAGER(const std::string Camera_Model_Name);

        /**
         * @brief Constructor to initialize the camera manager with additional camera settings.
         * @param camera_index The index of the camera to be used.
         * @param image_fps Desired frames per second for video capture.
         * @param image_Brightness Desired brightness level for the camera.
         * @param image_Contrast Desired contrast level for the camera.
         * @param image_Saturation Desired saturation level for the camera.
         * @param image_Frame_Resolution Desired resolution of the captured images (width and height).
         */
        CAMERA_MANAGER(uint8_t camera_index,
                       double image_fps,
                       double image_Brightness,
                       double image_Contrast,
                       double image_Saturation,
                       image_resolution_t image_Frame_Resolution);

        CAMERA_MANAGER(const std::string Camera_Model_Name,
                       double image_fps,
                       double image_Brightness,
                       double image_Contrast,
                       double image_Saturation,
                       image_resolution_t image_Frame_Resolution);
        ~CAMERA_MANAGER();
        /**
         * @brief Captures an image from the camera.
         * @return Status of the capture operation (success or failure).
         */
        fun_status_t Capture_image();
        /**
         * @brief Displays the most recently captured image in a window.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Show_image();
        /**
         * @brief Continuously displays video from the camera in a window.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Show_video();
        /**
         * @brief Starts the camera for capturing images or video.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Start_Camera();
        /**
         * @brief Stops the camera and releases any associated resources.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Stop_Camera();
        /**
         * @brief Retrieves the most recently captured image.
         * @param Captured_image Reference to a `cv::Mat` object to store the captured image.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Get_Captured_image(cv::Mat &Captured_image);
        /**
         * @brief Encodes the most recently captured image in Base64 format.
         * @param encoding_image Reference to a string to store the encoded image.
         * @return Status of the operation (success or failure).
         */
        fun_status_t Get_ImageBase64Encoding(std::string &encoding_image);

        camera_status_t Get_CameraStatuse();

        void Set_Image(cv::Mat &image);
    };

}
#endif // CLIENT_CAMERA_MANAGER_HPP
