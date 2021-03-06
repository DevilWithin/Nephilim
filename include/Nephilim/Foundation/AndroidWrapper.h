#ifndef NephilimFoundationAndroidWrapper_h__
#define NephilimFoundationAndroidWrapper_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Foundation/File.h>
#include "StringList.h"

#ifdef NEPHILIM_ANDROID
#include <jni.h>
#include <android/log.h>
#endif

NEPHILIM_NS_BEGIN

/**
	\class AndroidWrapper
	\brief Static class to interact with the android device

	 Please notice that this class is not meant to be used directly by the user. 
	 It is even omitted to the user completely for true cross platform usability.
	 For optimization it is not compiled in other platforms, as it wont be used.
*/
class NEPHILIM_API AndroidWrapper
{
public:
	/// \class AssetFile
	/// \brief Points to a file within the android packaged assets
	struct AssetFile
	{
		int fd;
		Int64 offset;
		Int64 length;
		bool success;
	};

	/// Fetch an asset file in the android file system
	/// Returns true or false wether it could find the file and access it
	/// The ScopedFile must be a pointer to a valid instance, and it will be overriden by the asset found
	/// path is the location of the asset, relative paths will be in the APK assets
	/// Absolute paths to external storage or data path are allowed and will work as well.
	/// The asset suffix will be applied to the path automatically if needed. Change it using setAssetSuffix()
	/// An absolute directory is recognized by the first character being a slash (  /  )
	/// binaryMode is meant to specify if you want to read in binary or text mode ( "rb" and "r" modes )
	static bool getAssetFile(File* file, const String &path, bool binaryMode = true);

	/// Searches for an asset and returns information about it
	static AssetFile getAsset(const String& filename);

	/// Get a listing of files/directories within the APK assets, empty path will mean the root directory of assets
	static StringList getAssetList(const String &path);

	/// Attempts to create a directory in the Android file system
	/// Make sure you have the needed permissions in the manifest
	static bool createDirectory(const String& path);

	/// Sends a text message through the android device, please note the permissions must be requested in the manifest and the legacy java source must be in place.
	static bool sendTextMessage(const String &destinationNumber, const String &content);
	
	/// Closes the activity, ending the application
	static void closeActivity();
	
	/// Get the current asset suffix
	static String getAssetSuffix();

	/// Set the asset suffix to be automatically applied to paths
	static void setAssetSuffix(const String &suffix);

	/// Get the android package name
	static String getPackageName();

	/// Set the android package name
	static void setPackageName(const String &name);

	/// Get the android activity name
	static String getActivityName();

	/// Request a frame render
	static void requestFrameRender();

	/// Enable multitouch support
	static void enableMultiTouch();	
	
	/// Disable multitouch support
	static void disableMultiTouch();

	/// Toggle the software keyboard
	static void toggleKeyboard();

	/// Play a music in the android
	static int playMusic(const String &name);

	/// Set the android activity name
	static void setActivityName(const String& name);

	/// Set the JNI environment the application is using
	static void setJavaNativeInterfaceEnvironment(JavaVM *environment);

	/// Set the directory of the external storage, the sdcard in case of the android
	static void setExternalStorageDirectory(const String& path);

	/// Get the external storage directory, the path to the sdcard root
	static String getExternalStorageDirectory();

	/// Vibrate the device for miliseconds
	static void vibrate(int miliseconds);

	/// Sets the internal data path
	static void setInternalDataPath(const String& path);

	/// Get the internal data path of the application, where the save files are stored
	static String getInternalDataPath();

private:
	static String m_externalStorageDirectory;
	/// Suffix to automatically append to file names
	/// For now, it is .png because all assets are suffixed with an additional .png to avoid apk compression.
	static String m_AssetSuffix;

	/// The android package name, usually its in the form of com.x.y.z
	static String m_packageName;

	/// The internal path to store save games
	static String m_internalDataPath;

	/// The name of the activity, as JNI understands it. Usually something like com/x/y/z/classname
	static String m_JNIActivityName;

	/// JNI virtual machine to get the environment
	static JavaVM* m_javaVM;
};

NEPHILIM_NS_END
#endif // NephilimFoundationAndroidWrapper_h__