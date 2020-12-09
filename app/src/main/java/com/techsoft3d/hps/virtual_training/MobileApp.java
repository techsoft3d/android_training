package com.techsoft3d.hps.virtual_training;

// Auto-generated file

public class MobileApp {
	private static native void shutdownV();
	private static native void setLibraryDirectoryS(String libraryDir);
	private static native void setFontDirectoryS(String fontDir);
	private static native void setMaterialsDirectoryS(String materialsDir);

	public static void shutdown() {
		 shutdownV();
	}


	public static void setLibraryDirectory(String libraryDir) {
		 setLibraryDirectoryS(libraryDir);
	}


	public static void setFontDirectory(String fontDir) {
		 setFontDirectoryS(fontDir);
	}


	public static void setMaterialsDirectory(String materialsDir) {
		 setMaterialsDirectoryS(materialsDir);
	}


}

