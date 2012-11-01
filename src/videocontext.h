class IVideoContext {
    public:
        virtual bool IsCurrent() = 0;
        virtual void MakeCurrent() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Close() = 0;
        virtual void GlobalImageTexture2D(GLenum target, void* global_image) = 0;
};
