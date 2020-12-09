package com.techsoft3d.hps.virtual_training;

import android.Manifest;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MobileSurfaceActivity extends Activity implements
        AndroidMobileSurfaceView.Callback {

    /* JNI Abstraction layer to HPS Visualize Canvas defined in UserMobileSurface.cpp */
    private AndroidUserMobileSurfaceView mSurfaceView;
    private View mUserCodeView;

    /* onCreate should load the native HOOPS .so files once. */
    private static boolean mNativeLibsLoaded = false;

    /* Detect if the Visualize Configuration should support HOOPS Exchange. */
    public static boolean USING_EXCHANGE = false;

    /* Store the file path for the HFS Model we load. */
    private String mPath = Environment.getExternalStorageDirectory().getPath() + "/" + "conrod.hsf";

    private boolean mShouldLoadFile = true;
    private ProgressDialog mProgress;

    // Layout for activity - will hold HOOPS Visualize SurfaceView
    private FrameLayout mMainLayout;

    // We only have 1 AndroidMobileSurfaceView in this app
    static final int MOBILE_SURFACE_GUI_ID = 0;

    // String used to store Surface pointer when activity needs to save state
    static final String MOBILE_SURFACE_POINTER_KEY = "mobileSurfaceId";

    /**
     * Copy the given file from "Assets" to the Virtual Devices /etc/storage/0
     */
    private void copyModels() {
        AssetManager assetManager = getAssets();

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
        }

        String filename = "datasets/conrod.hsf";

        InputStream in = null;
        OutputStream out = null;

        try {
            in = assetManager.open(filename);
                File outFile = new File(Environment.getExternalStorageDirectory().getPath(), "conrod.hsf");
                out = new FileOutputStream(outFile);

                // Copy In -> Out
                byte[] buffer = new byte[1024];
                int read;
                while ((read = in.read(buffer)) != -1) {
                    out.write(buffer, 0, read);
                }

                out.close();
        } catch (IOException e) {
            Log.e("Visualize_Training", "Unable to copy Model to SD Card.");
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        long mobileSurfacePointer = 0;

        /* If reloading saved state - don't reload the file. */
        if (savedInstanceState != null) {
            /*  - Set the surface pointer located in the Bundle instance state */
            mobileSurfacePointer = savedInstanceState.getLong(MOBILE_SURFACE_POINTER_KEY);
            mShouldLoadFile = false;
        } else {
            /* Otherwise start the progress dialog and Load File.. */
            mProgress = ProgressDialog.show(MobileSurfaceActivity.this, "", "Loading. Please wait...", true);
        }

        /* Copies the single model in use from Assets to the virtual devices FS. */
        copyModels();

        if(!mNativeLibsLoaded) {
            /* See if HOOPS Exchange Dependencies are present.  */
            File a3dlibs = new File(this.getApplicationInfo().nativeLibraryDir + "/libA3DLIBS.so");
            File sprk_exchange = new File(this.getApplicationInfo().nativeLibraryDir + "/libhps_sprk_exchange.so");

            USING_EXCHANGE = a3dlibs.exists() && sprk_exchange.exists();
            try {
                /* Library named after .so generated in CMakeLists.txt */
                System.loadLibrary("Visualize_Training");
            } catch (Error e) {
                System.out.println("Failed to load .so.");
            }

            mNativeLibsLoaded = true;
        }

        MobileApp.setLibraryDirectory(this.getApplicationInfo().nativeLibraryDir);

        /* Create Surface View dedicated to HOOPS Viz: https://developer.android.com/reference/android/view/SurfaceView */
        mSurfaceView = new AndroidUserMobileSurfaceView(this, this, MOBILE_SURFACE_GUI_ID, mobileSurfacePointer);

        /* Programatically create Android Layout and add View. Assign Main Activity Content. */
        mMainLayout = new FrameLayout(this);
        mMainLayout.addView(mSurfaceView);

        /* Add Toolbar buttons to invoke functionality. */
        // NOTE! Covering a SurfaceView object with layouts can result in decreased performance.
        mUserCodeView = getLayoutInflater().inflate(R.layout.user_code, null);
        mMainLayout.addView(mUserCodeView);

        setContentView(mMainLayout);
    }

    /**
     *  Connect XML ToolBar Buttons to HPS.
     *  Functionality defined in UserMobileSurface.cpp is compiled with sip.py into
     *  Java Native Interface (JNI) and exposed through AndroidUserMobileSurfaceView.java
     */
    public void toolbarButtonPressed(View view) {
        switch (view.getId()) {
            case R.id.userCode1Button:
                showToast("User code 1!");
                break;
            case R.id.userCode2Button:
                showToast("User code 2!");
                break;
            case R.id.userCode3Button:
                showToast("User code 3!");
                break;
            case R.id.userCode4Button:
                showToast("User code 4!");
                break;
        }
    }

    /**
     * Recover from Application being minimized or something of the such.
     * @param savedInstanceState
     */
    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);

        // Save our mobile surface pointer so that we can restore the Visualize Scene on reload.
        savedInstanceState.putLong(MOBILE_SURFACE_POINTER_KEY, mSurfaceView.getSurfacePointer());
    }

    /**
     * HPS Visualize Setup
     *  MobileSurface.cpp:16
     *  If bindRet == True
     *   - HPS Visualize "Canvas" loaded successfully and is ready for use.
     */
    @Override
    public void onSurfaceBind(boolean bindRet) {
        // Start load-file asynchronous task
        if (bindRet == false) {
            showToast("C++ bind() failed to initialize");
            return;
        }

        if (mShouldLoadFile) {
            showToast("Loading file.");
            new LoadFileAsyncTask().execute(mPath);
        }

    }

    // AsyncTask for performing file load on a separate thread.
    // Note that calling cancel() on this task will *not* abort the file load.
    private class LoadFileAsyncTask extends AsyncTask<String, Void[], Boolean> {
        @Override
        protected Boolean doInBackground(String... paths) {
            // Perform file load on separate thread
            return mSurfaceView.loadFile(paths[0]);
        }

        @Override
        protected void onPostExecute(Boolean result) {
            if (result == false)
                showToast("File failed to load");

            if (mProgress != null) {
                mProgress.dismiss();
                mProgress = null;
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSurfaceView.clearTouches();
    }

    private void showToast(String msg) {
        Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_LONG).show();
    }

    public void onShowPerformanceTestResult(float fps) {
        System.out.println("Show Performance Test Result.");
    }

    public void onShowKeyboard() {
        System.out.println("onShowKeyboard");
    }

    public void eraseKeyboardTriggerField() {
        System.out.println("EraseKeyboardTrigger.");
    }

    @Override
    public void onBackPressed() {
        mSurfaceView.surfaceDestroyed(mSurfaceView.getHolder());
        super.onBackPressed();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle menu option selection

        switch (item.getItemId()) {
            case android.R.id.home:
                // Go back to the SandboxFileList activity
                mSurfaceView.surfaceDestroyed(mSurfaceView.getHolder());
                finish();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
