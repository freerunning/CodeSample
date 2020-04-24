package android.ovum;

import android.util.Slog;
import android.view.Surface;

/**
 * Created by dingyu on 17-12-7.
 * {@hide}
 */

public class YuvRenderer {
    public static native void setSurface(int width, int height, Surface surface);
    public static native void render(byte[] data,int size);
}
