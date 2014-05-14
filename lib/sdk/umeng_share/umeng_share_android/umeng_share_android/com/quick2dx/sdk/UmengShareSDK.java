package com.quick2dx.sdk;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;

import android.content.Intent;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.controller.RequestType;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.media.UMVideo;
import com.umeng.socialize.media.UMusic;
import com.umeng.socialize.sso.QZoneSsoHandler;
import com.umeng.socialize.sso.SinaSsoHandler;
import com.umeng.socialize.sso.TencentWBSsoHandler;
import com.umeng.socialize.sso.UMSsoHandler;

public class UmengShareSDK {

    private UmengShareSDK() {

    }

    public static UmengShareSDK getInstance() {
        if (null == gInstance) {
            gInstance = new UmengShareSDK();
        }

        return gInstance;
    }

    public static void share(String text, String img, String audio, String video) {

    }

    public void init(Cocos2dxActivity activity) {
        mContext = activity;

        mController.getConfig().setSsoHandler(new QZoneSsoHandler(activity));
        mController.getConfig().setSsoHandler(new SinaSsoHandler());
        mController.getConfig().setSsoHandler(new TencentWBSsoHandler());
    }

    public static void setAppWebSite(final String shareMedia, final String webSite) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                UmengShareSDK shareSDK = getInstance();
                SHARE_MEDIA[] sm = shareSDK.str2SHAREMEDIA(shareMedia);
                if (null == sm) {
                    return ;
                }

                shareSDK.mController.setAppWebSite(sm[0], webSite);

            }

        });
    }

    public static void removePlatform(final String shareMedia) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                UmengShareSDK shareSDK = getInstance();
                SHARE_MEDIA[] sm = shareSDK.str2SHAREMEDIA(shareMedia);
                if (null == sm) {
                    return ;
                }

                shareSDK.mController.getConfig().removePlatform(sm);
            }
        });

    }

    public static void reorderPlatform(final String shareMedia) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                UmengShareSDK shareSDK = getInstance();
                SHARE_MEDIA[] sm = shareSDK.str2SHAREMEDIA(shareMedia);
                if (null == sm) {
                    return ;
                }

                shareSDK.mController.getConfig().setPlatformOrder(sm);
            }

        });

    }

    public static void shareText(final String text) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                getInstance().mController.setShareContent(text);

                getInstance().mController.openShare(mContext, false);
            }

        });
    }

    public static void shareImg(final String text, final String img, final int imgType) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                getInstance().mController.setShareContent(text);

                switch (imgType) {
                    case 0: {
                        //local resource
                        int nRes = Integer.valueOf(img).intValue();
                        getInstance().mController.setShareMedia(new UMImage(mContext, nRes));
                        break;
                    }
                    case 1: {
                        //local path
                        getInstance().mController.setShareMedia(new UMImage(mContext,
                                BitmapFactory.decodeFile(img)));
                        break;
                    }
                    case 2: {
                        //web url
                        getInstance().mController.setShareMedia(new UMImage(mContext,
                                img));
                        break;
                    }
                    default: {
                        break;
                    }
                }

                getInstance().mController.openShare(mContext, false);
            }
        });
    }

    public static void shareMusic(final String text, final String musicFile, final String musicimg, final String author, final String title) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                getInstance().mController.setShareContent(text);

                // 设置分享音乐
                UMusic uMusic = new UMusic(musicFile);
                uMusic.setAuthor(author);
                uMusic.setTitle(title);
                // 设置音乐缩略图
                uMusic.setThumb(musicimg);
                getInstance().mController.setShareMedia(uMusic);

                getInstance().mController.openShare(mContext, false);

            }

        });

    }

    public static void shareVideo(final String text, final String videoFile, final String videoImg, final String title) {
        mContext.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                getInstance().mController.setShareContent(text);

                // 设置分享视频
                UMVideo umVideo = new UMVideo(videoFile);
                // 设置视频缩略图
                umVideo.setThumb(videoImg);
                umVideo.setTitle(title);
                getInstance().mController.setShareMedia(umVideo);

                getInstance().mController.openShare(mContext, false);

            }

        });

    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        /**使用SSO授权必须添加如下代码 */
        UMSsoHandler ssoHandler = mController.getConfig().getSsoHandler(requestCode) ;
        if(ssoHandler != null){
            ssoHandler.authorizeCallBack(requestCode, resultCode, data);
        }
    }

    private SHARE_MEDIA[] str2SHAREMEDIA(String shareMedia) {
        String[] arrStr = shareMedia.split(",");

        SHARE_MEDIA[] sm = new SHARE_MEDIA[arrStr.length];
        try {
            for (int i = 0; i < arrStr.length; i++) {
                sm[i] = SHARE_MEDIA.valueOf(arrStr[i]);
            }
        } catch(Exception e) {
            sm = null;
            Log.e("UmengShareSDK", "trans SHARE_MEDIA fail");
        }

        return sm;
    }

    private boolean strIsNull(String str) {
        if (null == str) {
            return true;
        }
        if (0 == str.length()) {
            return true;
        }
        return false;
    }

    public static void addScriptListener(final int listener_) {
        removeScriptListener();
        mListener = listener_;
    }

    public static void removeScriptListener() {
        if (mListener != 0) {
            mContext.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxLuaJavaBridge.releaseLuaFunction(mListener);
                    mListener = 0;
                }
            });
        }
    }

    //on CocoPushReceiver callback
    public void onReceiverInfo(final String tags) {
        if (0 == mListener) {
            return ;
        }

        mContext.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxLuaJavaBridge.callLuaFunctionWithString(mListener, tags);
            }
        });
    }

    private static UmengShareSDK gInstance;
    private static Cocos2dxActivity mContext;
    private static int mListener;
    final UMSocialService mController = UMServiceFactory.getUMSocialService("com.quick2dx.sdk",
            RequestType.SOCIAL);
}
