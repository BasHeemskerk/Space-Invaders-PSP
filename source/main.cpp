#include "included/global.hpp"

//global
#include "included/state.hpp"
#include "included/sprite.hpp"


//game
#include "included/player.hpp"


using namespace std;

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) 
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2)

PSP_MODULE_INFO("Space Invaders", 0, 1, 0);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1024);

#if _PSP_FW_VERSION >= 200
PSP_HEAP_SIZE_KB(20480);
#endif

static unsigned int __attribute__((aligned(16))) DisplayList[262144];

int exit_callback(int arg1, int arg2, void* common){
    sceKernelExitGame();
    return 0;
}

int callbackThread(SceSize args, void* argp){
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();
    return 0;
}

void setupCallbacks(){
    int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, NULL);
    if (thid >= 0){
        sceKernelStartThread(thid, 0, NULL);
    }
}

void* fbp0 = NULL;
void* fbp1 = NULL;
void* renderTarget = NULL;
void* zbp = NULL;

void InitGU(){    
    fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
    fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
    zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

    renderTarget = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);

    sceGuInit();
    sceGuStart(GU_DIRECT,DisplayList);

    sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
    sceGuDepthBuffer(zbp,BUF_WIDTH);

    sceGuDepthRange(65535,0);
    sceGuDepthMask(GU_FALSE);

    sceGuOffset(2048 - (SCR_WIDTH / 2),2048 - (SCR_HEIGHT / 2));

    sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);

    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

    sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);

    sceGuAlphaFunc(GU_GREATER,0,0xff);
    sceGuEnable(GU_ALPHA_TEST);

    sceGuDepthFunc(GU_GEQUAL);
    sceGuEnable(GU_DEPTH_TEST);

    sceGuFrontFace(GU_CW);

    sceGuTexMode(GU_PSM_5650,0,0,0); 
    sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
    sceGuTexFilter(GU_NEAREST,GU_NEAREST);

    sceGuEnable(GU_CULL_FACE);
    
    sceGuEnable(GU_TEXTURE_2D);

    sceGuEnable(GU_DITHER);

    sceGuEnable(GU_CLIP_PLANES);
    
    sceGuEnable(GU_LIGHTING);
    sceGuEnable(GU_LIGHT0);
    sceGuEnable(GU_LIGHT1);
    sceGuEnable(GU_LIGHT2);
    sceGuEnable(GU_LIGHT3);
    
    sceGuEnable(GU_FOG);

    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);

    sceGuShadeModel(GU_SMOOTH); 

    sceGuFinish();

    sceGuSync(0,0);

    sceDisplayWaitVblankStart();

    sceGuDisplay(GU_TRUE);
}


void initEngine(){
    setupCallbacks();
    pspDebugScreenInit();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    InitGU();

    //oslInit();
    VirtualFileInit();
    oslInitAudio();
}

void initGame(){

}


auto main() -> int{

    
    SceCtrlData ctrlData;

    initEngine();

    while(true){

        sceCtrlReadBufferPositive(&ctrlData, 1);

        sceDisplayWaitVblankStart();

        //running = isRunning();

        sceGuStart(GU_DIRECT,DisplayList);

        // clear screen

        sceGuClearColor(0x000000);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        // setup aspect ratio

        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumPerspective(70.0f,16.0f/9.0f,0.5f,1000.0f);
            
            // setup camera
        sceGumMatrixMode(GU_VIEW);
        {
            ScePspFVector3 pos = {0, 0, 0};
            ScePspFVector3 rot = { 0, 0, 0 };

            sceGumLoadIdentity();
            sceGumTranslate(&pos);
            sceGumRotateXYZ(&rot);
        }

        if (state::alive == true){

        }
        
            
        sceGuFinish();
        sceGuSync(0,0);

        sceGuSwapBuffers();
    }
}