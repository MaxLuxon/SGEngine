#import "GameViewController.h"

@interface GameViewController () {}

@property (strong, nonatomic) EAGLContext *context;
- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [view bindDrawable];
    [self setupGL];
 
   // [view bindDrawable];
    
    self.preferredFramesPerSecond = 60;
        
    swidth = [UIScreen mainScreen].bounds.size.width;
    sheight = [UIScreen mainScreen].bounds.size.height;

    sge::Director::getInstance()->setWindowSize(sge::Vec2(swidth*2.0,sheight*2.0));
    sge::Director::getInstance()->init();
    
    sge::Application::run(&appDelegate, "assets/config/main.ini");
    
    checkRenderError();

}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL{
    
    [EAGLContext setCurrentContext:self.context];

    
}

- (void)tearDownGL{
    
    [EAGLContext setCurrentContext:self.context];
    
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint location = [touch locationInView:self.view];
    sge::Director::getInstance()->mouseDown(sge::Vec2(location.x, sheight-location.y), false);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint location = [touch locationInView:self.view];
    sge::Director::getInstance()->mouseMoved(sge::Vec2(location.x, sheight-location.y), false);
    
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint location = [touch locationInView:self.view];
    sge::Director::getInstance()->mouseUp(sge::Vec2(location.x, sheight-location.y), false);
}




#pragma mark - GLKView and GLKViewController delegate methods

- (void)update{

    sge::Director::getInstance()->update(self.timeSinceLastUpdate*10.0);

}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{

    sge::Director::getInstance()->draw();

}

@end
