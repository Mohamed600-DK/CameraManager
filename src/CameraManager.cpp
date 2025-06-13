
#ifdef _WIN32
#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mfobjects.h>
#endif
#ifdef DEBUG_MODE
#include <iostream>
#endif
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/registry.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "CamerManager.hpp"


using namespace CameraManager;
// Constants for default camera settings
constexpr double DEFAULT_FPS = 30.0;
constexpr double DEFAULT_BRIGHTNESS = 50.0;
constexpr double DEFAULT_CONTRAST = 50.0;
constexpr double DEFAULT_SATURATION = 50.0;
constexpr image_resolution_t DEFAULT_RESOLUTION = SD;
constexpr uint8_t Max_Camera_indexs = 10;
// Resolution dimensions (width and height for various resolutions)
constexpr uint8_t WIDTH = 0;
constexpr uint8_t HIGHT = 1;
constexpr uint16_t RESOLUTION_TABLE[4][2] = {
	/*SD*/ {640, 480},
	/*HD*/ {1280, 720},
	/*FHD*/ {1920, 1080}};

// Base64 encoding character set
constexpr char base64_chars[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

static std::string base64_encode(const std::vector<uint8_t> &binary_data);

class CAMERA_MANAGER::ClassAttributes
{
public:
	uint8_t att_camera_id;					 ///< ID/index of the camera to be managed.
	image_properties_t att_image_Properties; ///< Properties of the captured image (e.g., resolution, brightness).
	cv::Mat att_capture_image;				 ///< Matrix to store the captured image.
	cv::VideoCapture att_video_capture;		 ///< OpenCV VideoCapture object for camera operations.
	uint32_t att_backend_video_API;
	camera_status_t att_camera_statuse;
	~ClassAttributes() = default;
	ClassAttributes() = default;

};
CAMERA_MANAGER::~CAMERA_MANAGER() = default;
/**
 * @brief Construct a new camera manager::camera manager object
 * 
 * @param camera_index 
 */
CAMERA_MANAGER::CAMERA_MANAGER(uint8_t camera_index) : class_attribute(std::make_unique<ClassAttributes>())
{
	class_attribute->att_camera_id = camera_index;
	class_attribute->att_image_Properties = {DEFAULT_FPS, DEFAULT_BRIGHTNESS, DEFAULT_CONTRAST, DEFAULT_SATURATION, DEFAULT_RESOLUTION};
	class_attribute->att_backend_video_API = cv::CAP_ANY;
	class_attribute->att_camera_statuse = CAMERA_CLOSE;
}

CAMERA_MANAGER::CAMERA_MANAGER(const std::string Camera_Model_Name) : class_attribute(std::make_unique<ClassAttributes>())
{

	class_attribute->att_camera_id = FindCameraIndexByModelName(Camera_Model_Name);
	class_attribute->att_image_Properties = {DEFAULT_FPS, DEFAULT_BRIGHTNESS, DEFAULT_CONTRAST, DEFAULT_SATURATION, DEFAULT_RESOLUTION};
	class_attribute->att_backend_video_API = cv::CAP_ANY;
	class_attribute->att_camera_statuse = CAMERA_CLOSE;
}

/**
 * @brief Construct a new camera manager::camera manager object
 * 
 * @param camera_index 
 * @param image_fps 
 * @param image_Brightness 
 * @param image_Contrast 
 * @param image_Saturation 
 * @param image_Frame_Resolution 
 */
CAMERA_MANAGER::CAMERA_MANAGER(uint8_t camera_index, double image_fps, double image_Brightness, double image_Contrast, double image_Saturation, image_resolution_t image_Frame_Resolution) : class_attribute(std::make_unique<ClassAttributes>())
{
	class_attribute->att_camera_id = camera_index;
	class_attribute->att_image_Properties = {image_fps, image_Brightness, image_Contrast, image_Saturation, image_Frame_Resolution};
	class_attribute->att_backend_video_API = cv::CAP_ANY;
	class_attribute->att_camera_statuse = CAMERA_CLOSE;
}
/**
 * @brief Construct a new camera manager::camera manager object
 * 
 * @param Camera_Model_Name 
 * @param image_fps 
 * @param image_Brightness 
 * @param image_Contrast 
 * @param image_Saturation 
 * @param image_Frame_Resolution 
 */
CAMERA_MANAGER::CAMERA_MANAGER(const std::string Camera_Model_Name, double image_fps, double image_Brightness, double image_Contrast, double image_Saturation, image_resolution_t image_Frame_Resolution)
	: class_attribute(std::make_unique<ClassAttributes>())
{

	class_attribute->att_camera_id = FindCameraIndexByModelName(Camera_Model_Name);
	class_attribute->att_image_Properties = {image_fps, image_Brightness, image_Contrast, image_Saturation, image_Frame_Resolution};
	class_attribute->att_backend_video_API = cv::CAP_ANY;
	class_attribute->att_camera_statuse = CAMERA_CLOSE;
}

/**
 * @brief 
 * 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Capture_image()
{
	fun_status_t local_Fun_Statuse;
	if (Get_CameraStatuse() == CAMERA_OPEN)
	{
		// Check if the camera is open before attempting to capture
		if (class_attribute->att_video_capture.read(class_attribute->att_capture_image))
		{
			local_Fun_Statuse = FUN_OK;
		class_attribute->att_camera_statuse = CAMERA_OPEN;
		}
		else
		{
			local_Fun_Statuse = FUN_ERROR;
		class_attribute->att_camera_statuse = CAMERA_CLOSE;
		class_attribute->att_capture_image = cv::Mat();
		}
	}
	else
	{
		local_Fun_Statuse = FUN_ERROR;
	class_attribute->att_camera_statuse = CAMERA_CLOSE;
	class_attribute->att_capture_image = cv::Mat();
	}

	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @param Captured_image 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Get_Captured_image(cv::Mat &Captured_image)
{
	fun_status_t local_Fun_Statuse = FUN_ERROR;

	// Ensure the captured image is valid before copying
	if (!class_attribute->att_capture_image.empty())
	{
		Captured_image = class_attribute->att_capture_image.clone();
		local_Fun_Statuse = FUN_OK;
	}
	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Show_image()
{
	fun_status_t local_Fun_Statuse = FUN_ERROR;

	// Check if there is a valid image to display
	if (!class_attribute->att_capture_image.empty())
	{
		cv::imshow("Current_Image", class_attribute->att_capture_image);
		if (cv::waitKey(0) > 0)
		{ // Wait for user key press
			local_Fun_Statuse = FUN_OK;
		}
	}
	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Show_video()
{
	fun_status_t local_Fun_Statuse = FUN_OK;

	// Continuously capture and display frames
	while (true)
	{
		if (Capture_image() == FUN_OK)
		{
			cv::imshow("Video", class_attribute->att_capture_image);
			if (cv::waitKey(1) == 'q')
			{ // Exit loop on 'q' key press
				break;
			}
		}
		else
		{
			local_Fun_Statuse = FUN_ERROR;
			break;
		}
	}
	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Start_Camera()
{
	fun_status_t local_Fun_Statuse = FUN_ERROR;
#ifdef DEBUG_MODE
	std::cout << "****************************" << std::endl;
	std::cout << "Available backends: " << std::endl;

	for (auto &i : cv::videoio_registry::getBackends())
	{
		std::cout << ">> " << "{" << i << "}" << cv::videoio_registry::getBackendName(i) << std::endl;
	}
	std::cout << "****************************" << std::endl;
#endif

#ifdef _WIN32
class_attribute->att_backend_video_API = cv::CAP_DSHOW;
#endif
#ifdef __linux__
class_attribute->att_backend_video_API = cv::CAP_V4L2;
#endif
	// Attempt to open the camera
	if (class_attribute->att_video_capture.open(class_attribute->att_camera_id,class_attribute->att_backend_video_API))
	{

#ifdef DEBUG_MODE
		std::cout << "Found Camera at Index: " << static_cast<int>(class_attribute->att_camera_id) << std::endl;
#endif
		// ToDo: Make this In sperted  method
		//  Set camera properties
		if (class_attribute->att_image_Properties.Image_FPS != 0)
		{
			class_attribute->att_video_capture.set(cv::CAP_PROP_FPS,class_attribute->att_image_Properties.Image_FPS);
		}
		if (class_attribute->att_image_Properties.Image_Brightness != 0)
		{
			class_attribute->att_video_capture.set(cv::CAP_PROP_BRIGHTNESS, class_attribute->att_image_Properties.Image_Brightness);
		}
		if (class_attribute->att_image_Properties.Image_Contrast != 0)
		{
			class_attribute->att_video_capture.set(cv::CAP_PROP_CONTRAST, class_attribute->att_image_Properties.Image_Contrast);
		}
		if (class_attribute->att_image_Properties.Image_Saturation != 0)
		{
			class_attribute->att_video_capture.set(cv::CAP_PROP_SATURATION, class_attribute->att_image_Properties.Image_Saturation);
		}
		if (class_attribute->att_image_Properties.Image_Frame_Resolution != NULL_RES)
		{
			class_attribute->att_video_capture.set(cv::CAP_PROP_FRAME_WIDTH, RESOLUTION_TABLE[class_attribute->att_image_Properties.Image_Frame_Resolution][WIDTH]);
			class_attribute->att_video_capture.set(cv::CAP_PROP_FRAME_HEIGHT, RESOLUTION_TABLE[class_attribute->att_image_Properties.Image_Frame_Resolution][HIGHT]);
		}
		local_Fun_Statuse = FUN_OK;
		class_attribute->att_camera_statuse = CAMERA_OPEN;
	}
	else
	{
#ifdef DEBUG_MODE
		std::cout << class_attribute->att_video_capture.get(cv::CAP_PROP_BACKEND) << std::endl;
		std::cout << "Not Found Camera at Index: " << static_cast<int>(class_attribute->att_camera_id) << std::endl;
#endif
		if (class_attribute->att_camera_id < Max_Camera_indexs)
		{
			class_attribute->att_camera_id++;
		}
		class_attribute->att_camera_statuse = CAMERA_CLOSE;
		local_Fun_Statuse = FUN_ERROR;
	}
	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Stop_Camera()
{
	fun_status_t local_Fun_Statuse = FUN_OK;

	// Release the camera if it is open
	if (class_attribute->att_camera_statuse == CAMERA_OPEN)
	{
		class_attribute->att_video_capture.release();
	}
	class_attribute->att_camera_statuse = CAMERA_CLOSE;

	return local_Fun_Statuse;
}

/**
 * @brief 
 * 
 * @param binary_data 
 * @return std::string 
 */
// Function to encode binary data to Base64
std::string base64_encode(const std::vector<uint8_t> &binary_data)
{
	std::string encoded;
	int val = 0, valb = -6;

	for (uint8_t c : binary_data)
	{
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0)
		{
			encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6)
		encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	while (encoded.size() % 4)
	{
		encoded.push_back('='); // Ensure correct padding
	}
	return encoded;
}

/**
 * @brief 
 * 
 * @param encoding_image 
 * @return fun_status_t 
 */
fun_status_t CAMERA_MANAGER::Get_ImageBase64Encoding(std::string &encoding_image)
{
	fun_status_t local_Fun_Statuse = FUN_OK;
	std::vector<uint8_t> binary_image;
	if (!class_attribute->att_capture_image.empty())
	{
		// Compress the image to JPEG format
		if (!cv::imencode(".jpg", class_attribute->att_capture_image.clone(), binary_image))
		{
#ifdef DEBUG_MODE
			std::cerr << "Failed to encode image!" << std::endl;
#endif
			return FUN_ERROR;
		}
		// Check if the last two bytes are correct (JPEG End Marker)
		if (binary_image.size() >= 2)
		{
#ifdef DEBUG_MODE
			std::cerr << "Last two bytes before Base64 encoding:" << binary_image[binary_image.size() - 2] << " " << binary_image[binary_image.size() - 1] << std::endl;
#endif
		}
		// Convert to Base64 using the fixed function
		encoding_image = base64_encode(binary_image);
	}
	else
	{
		local_Fun_Statuse = FUN_ERROR;
	}
	return local_Fun_Statuse;
}
/**
 * @brief 
 * 
 * @param targetModel 
 * @return int32_t 
 */
int32_t CAMERA_MANAGER::FindCameraIndexByModelName(const std::string &targetModel)
{
#ifdef _WIN32
	// Initialize Media Foundation
	HRESULT hr = MFStartup(MF_VERSION);
	int32_t cameraIndex = -1;
	if (FAILED(hr))
	{
#ifdef DEBUG_MODE
		std::cerr << "Failed to initialize Media Foundation!" << std::endl;
#endif
		return cameraIndex;
	}

	IMFAttributes *attributes = nullptr;
	IMFActivate **devices = nullptr;
	UINT32 deviceCount = 0;

	// Create an attribute store for enumeration
	hr = MFCreateAttributes(&attributes, 1);
	if (FAILED(hr))
	{
#ifdef DEBUG_MODE
		std::cerr << "Failed to create Media Foundation attributes!" << std::endl;
#endif
		return cameraIndex;
	}

	// Specify that we want to enumerate video capture devices
	hr = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if (FAILED(hr))
	{
#ifdef DEBUG_MODE
		std::cerr << "Failed to set GUID for enumeration!" << std::endl;
#endif
		attributes->Release();
		return cameraIndex;
	}

	// Enumerate devices
	hr = MFEnumDeviceSources(attributes, &devices, &deviceCount);
	if (FAILED(hr))
	{
#ifdef DEBUG_MODE
		std::cerr << "Failed to enumerate devices!" << std::endl;
#endif
		attributes->Release();
		return cameraIndex;
	}
	// Search for the target camera by model name
	for (UINT32 i = 0; i < deviceCount; ++i)
	{
		WCHAR *name = nullptr;
		UINT32 nameLength = 0;

		hr = devices[i]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &name, &nameLength);
		if (SUCCEEDED(hr))
		{
			std::wstring ws(name);
			std::string deviceName(ws.begin(), ws.end());
			if (deviceName.find(targetModel) != std::string::npos)
			{
				cameraIndex = i;
			}
			CoTaskMemFree(name);
		}
		devices[i]->Release();
		if (cameraIndex != -1)
			break; // Stop searching once found
	}

	attributes->Release();
	if (devices)
		CoTaskMemFree(devices);
	MFShutdown();
	return cameraIndex; // Camera not found
#elif __linux__
	std::string command_result = "";
	std::string videoDevice = "";
	std::string command = "";
	FILE *pipe = nullptr;
	char buffer[128];
	int32_t status = 0;
	for (int32_t i = 0; i < 5; ++i)
	{ // Adjust range if needed
		status = 0;
		memset(buffer, 0, sizeof(buffer));
		command_result = "";
		videoDevice = "/dev/video" + std::to_string(i);
		command = "udevadm info --query=all --name=" + videoDevice + " | grep ID_MODEL=";
		// Open a pipe to read the command's output
		pipe = popen(command.c_str(), "r");
		if (!pipe)
		{
#ifdef DEBUG_MODE
			std::cerr << "Failed to open pipe." << std::endl;
#endif
			return -1;
		}
		// Read the output line by line
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		{
			command_result += buffer; // Print the command's output
		}
		status = pclose(pipe);
		if (status != 0)
		{
			break;
		}
		// Check if the output contains the target model
		if (command_result.find(targetModel) != std::string::npos)
		{
			return i; // Found the camera
		}
	}
	return -1; // Camera not found
#else
#error "Unsupported platform"
#endif
}
/**
 * @brief 
 * 
 * @return camera_status_t 
 */
camera_status_t CAMERA_MANAGER::Get_CameraStatuse()
{
	if (class_attribute->att_video_capture.isOpened())
	{
		class_attribute->att_camera_statuse = CAMERA_OPEN;
	}
	else
	{
		class_attribute->att_camera_statuse = CAMERA_CLOSE;
		class_attribute->att_capture_image.release();
	}
	return class_attribute->att_camera_statuse;
}
/**
 * @brief 
 * 
 * @param image 
 */
void CAMERA_MANAGER::Set_Image(cv::Mat &image)
{
	if (!image.empty())
	{
		class_attribute->att_capture_image = image.clone();
	}
}
