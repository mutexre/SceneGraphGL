//
//  GLView.m
//  Hyperspace
//
//  Created by mutexre on 12/05/2017.
//
//

#import <OpenGL/OpenGL.h>
#import <SceneGraphGL/SceneGraphGL.hpp>
#import "GLView.hpp"

using namespace std;

@implementation GLView
{
    NSOpenGLContext* glContext;
    NSOpenGLPixelFormat* pixelFormat;
    CVDisplayLinkRef displayLink;
    shared_ptr<SG::GL::Context> context;
    shared_ptr<SG::Scene> scene;
}

- (instancetype)init
{
    if (self = [super init])
    {
        [self setupPixelFormat];
        [self setupGLContext];
        [self setupDisplayLink];
        [self setupScene];

        _layer.backgroundColor = CGColorCreateGenericRGB(1, 0, 0, 1);
    }
    
    return self;
}

- (void)dealloc {
    CVDisplayLinkRelease(displayLink);
}

- (void)setupPixelFormat
{
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAMultisample,
        NSOpenGLPFASamples, 1,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
        0
    };

    pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
}

- (void)setupGLContext
{
    glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    
    GLint val = 1;
    [glContext setValues:&val forParameter:NSOpenGLContextParameterSwapInterval];
    [glContext setValues:&val forParameter:NSOpenGLContextParameterSurfaceOpacity];
    
    glContext.view = self;
}

- (void)setupDisplayLink
{
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputHandler(displayLink, ^CVReturn(CVDisplayLinkRef  _Nonnull displayLink, const CVTimeStamp * _Nonnull inNow, const CVTimeStamp * _Nonnull inOutputTime, CVOptionFlags flagsIn, CVOptionFlags * _Nonnull flagsOut) {
        [self drawAndPresentToScreen];
        return kCVReturnSuccess;
    });
}

- (void)setupScene
{
    context = make_shared<SG::GL::Context>(SG::OS::macOS, SG::GL::API::CoreProfile, 4, 0);
    context->init();
    
    scene = context->createScene();
    scene->init();
    
    SG::Settings s;
    s.depthTest = true;
    s.depthClamp = true;
    s.writeDepth = true;
    s.seamlessCubemaps = true;
    s.smoothPoints = true;
    s.cullFace = SG::Settings::CullFace();
    s.blend = SG::Settings::Blend(SG::Settings::Blend::Func::srcAlpha,
                                  SG::Settings::Blend::Func::oneMinusSrcAlpha);
    s.scissor = SG::Settings::Scissor();
    s.colorMask = SG::Settings::ColorMask();
    
    scene->getRoot()->setSettings(s);
}

- (void)awakeFromNib {
    [self setWantsBestResolutionOpenGLSurface:YES];
}

- (void)layout
{
    [super layout];
    [glContext makeCurrentContext];
    [glContext update];
//    delegate?.glView(self, didResize: self.frame.size)
//    print(self.frame.size.width, self.frame.size.height, self.frame.size.width / self.frame.size.height);
}

//- (BOOL)preservesContentDuringLiveResize {
//    return NO;
//}
//
//- (void)viewWillStartLiveResize {
////    delegate?.glViewLiveResizeStarted(self)
//}
//
//- (void)viewDidEndLiveResize {
////    delegate?.glViewLiveResizeStopped(self)
//}

//- (void)makeContextCurrent {
//    [context makeCurrentContext];
//}

- (BOOL)acceptsFirstResponder {
    return YES;
}

// MARK: -

- (void)setUpdatesContinuously:(BOOL)updatesContinuously
{
    if (updatesContinuously)
        CVDisplayLinkStart(displayLink);
    else
        CVDisplayLinkStop(displayLink);
}

- (void)drawScene
{
    if (scene)
        scene->draw();
}

- (void)drawAndPresentToScreen
{
    [self drawAndPresentToScreen:^{
        [self drawScene];
    }];
}

- (void)drawAndPresentToScreen:(void(^)())block
{
    [glContext makeCurrentContext];
    block();
    [self presentToScreen];
}

- (void)presentToScreen
{
    [glContext flushBuffer];
}

@end
