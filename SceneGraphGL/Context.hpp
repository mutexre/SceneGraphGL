//
//  Created by mutexre on 17/01/2017.
//  Copyright © 2017 Some Organization. All rights reserved.
//

namespace SG::GL
{
    class Context : public SG::Context
    {
    private:
        API api;
        int version[2];
    
    protected:
        void fetchMaxMSAASamples();
        
        virtual shared_ptr<SG::Program> loadProgram(const char* vertexShaderName,
                                                    const char* fragmentShaderName) override;
        
        virtual shared_ptr<SG::Program> cloneProgram(const char* name) override;
        
        Settings::CullFace fetchCullFaceSettings();
        Settings::Blend fetchBlendSettings();
        Settings::Scissor fetchScissorSettings();
        
        void resetErrorFlags();
        
    public:
        Context(OS, API, int majorVersion, int minorVersion);
    
        API getAPI() const;
        int getVersion(int) const;
    
        bool isCoreProfileOrES3() const;
        bool isWrapRSupported() const;
    
    #pragma mark - Factory Methods
    
        virtual shared_ptr<SG::Scene> createScene() override;
        virtual shared_ptr<SG::Node> createNode() override;
        virtual shared_ptr<SG::MeshNode> createMeshNode() override;
        virtual shared_ptr<SG::Light> createLight() override;
        virtual shared_ptr<SG::Camera> createCamera() override;
        virtual shared_ptr<SG::Mesh> createMesh() override;
        virtual shared_ptr<SG::Texture> createTexture() override;
        virtual shared_ptr<SG::Surface> createSurface() override;
        virtual shared_ptr<SG::Renderbuffer> createRenderbuffer() override;
        virtual shared_ptr<SG::OffScreenBuffer> createOffScreenBuffer() override;
        
    #pragma mark - Settings Fetch
    
        virtual Settings fetchCurrentSettings() override;
        
    #pragma mark - Settings Apply
        
        virtual bool applyDepthTestSettings(bool) override;
        virtual bool applyDepthClampSettings(bool) override;
        virtual bool applyWriteDepthSettings(bool) override;
        virtual bool applySeamlessCubemapsSettings(bool) override;
        virtual bool applySmoothPointsSettings(bool) override;
        virtual bool applyCullFaceSettings(const Settings::CullFace&) override;
        virtual bool applyBlendSettings(const Settings::Blend&) override;
        virtual bool applyViewportSettings(ivec4) override;
        virtual bool applyScissorSettings(const Settings::Scissor&) override;
        virtual bool applyColorMaskSettings(const Settings::ColorMask&) override;
    };
}
