// #include "stdafx.h"      // this should be specified in the "Always included" entry in the project property
#include "GLUtil.h"

namespace KLIB {

GLUtil::BoolState GLUtil::boolState__;

//void TextureObject::copy_cpu2gpu(FramebufferObject& framebuffer, const GLvoid* pixels) {
void TextureObject::copy_cpu2gpu(GLenum type, const GLvoid* pixels) {
    // for NVIDIA
    bind();
    glTexSubImage2D(target_, 0, 0, 0, width_, height_, format_, type, pixels);
    unbind();
    //// for ATI
    //framebuffer.bind();
    //framebuffer.attachTexture(GL_COLOR_ATTACHMENT0_EXT, *this);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    //glRasterPos2i(0, 0);
    //glDrawPixels(width_, height_, format_, type_, pixels);
    //framebuffer.detachTexture(GL_COLOR_ATTACHMENT0_EXT);
    //framebuffer.unbind();
    GLUtil::checkError("TextureObject::copy_cpu2gpu");
}
//void TextureObject::copy_gpu2cpu(FramebufferObject& framebuffer, GLvoid* pixels) {
void TextureObject::copy_gpu2cpu(GLenum type, GLvoid* pixels) {
    //// one method (recommended)
    //framebuffer.bind();
    //GLUtil::checkError("hoge");
    //framebuffer.attachTexture(GL_COLOR_ATTACHMENT0_EXT, *this);
    //GLUtil::checkError("hoge");
    //glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    //GLUtil::checkError("hoge");
    //glReadPixels(0, 0, width_, height_, format_, type_, pixels);
    //GLUtil::checkError("hoge");
    //framebuffer.detachTexture(GL_COLOR_ATTACHMENT0_EXT);
    //GLUtil::checkError("hoge");
    //framebuffer.unbind();
    
    // another method
    bind();
    glGetTexImage(target_, 0, format_, type, pixels);    
    unbind();
    GLUtil::checkError("TextureObject::copy_gpu2cpu");
}
void TextureObject::copy_gpu2gpu(TextureObject& dst) {
    assert(is_compatible(dst));
    FramebufferObject fbo;
    fbo.init();
    fbo.bind();
    fbo.attachTexture(GL_COLOR_ATTACHMENT0_EXT, *this);
    GLUtil::checkFramebufferStatus();
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    dst.bind();
    glCopyTexImage2D(target_, 0, internalformat_, 0, 0, width_, height_, 0);
    dst.unbind();
    fbo.unbind();
    GLUtil::checkError("TextureObject::copy_gpu2gpu");
}


void glColor3d (const KLIB::Vector3d & c) { glColor3dv (c.ptr()); }
void glColor3f (const KLIB::Vector3f & c) { glColor3fv (c.ptr()); }
void glColor3b (const KLIB::Vector3c & c) { glColor3bv (c.ptr()); }
void glColor3i (const KLIB::Vector3i & c) { glColor3iv (c.ptr()); }
void glColor3ub(const KLIB::Vector3uc& c) { glColor3ubv(c.ptr()); }
void glColor3ui(const KLIB::Vector3ui& c) { glColor3uiv(c.ptr()); }
void glColor4d (const KLIB::Vector4d & c) { glColor4dv (c.ptr()); }
void glColor4f (const KLIB::Vector4f & c) { glColor4fv (c.ptr()); }
void glColor4b (const KLIB::Vector4c & c) { glColor4bv (c.ptr()); }
void glColor4i (const KLIB::Vector4i & c) { glColor4iv (c.ptr()); }
void glColor4ub(const KLIB::Vector4uc& c) { glColor4ubv(c.ptr()); }
void glColor4ui(const KLIB::Vector4ui& c) { glColor4uiv(c.ptr()); }

void glVertex2d(const KLIB::Vector2d& v) { glVertex2dv(v.ptr()); }
void glVertex2f(const KLIB::Vector2f& v) { glVertex2fv(v.ptr()); }
void glVertex2i(const KLIB::Vector2i& v) { glVertex2iv(v.ptr()); }
void glVertex3d(const KLIB::Vector3d& v) { glVertex3dv(v.ptr()); }
void glVertex3f(const KLIB::Vector3f& v) { glVertex3fv(v.ptr()); }
void glVertex3i(const KLIB::Vector3i& v) { glVertex3iv(v.ptr()); }
void glVertex4d(const KLIB::Vector4d& v) { glVertex4dv(v.ptr()); }
void glVertex4f(const KLIB::Vector4f& v) { glVertex4fv(v.ptr()); }
void glVertex4i(const KLIB::Vector4i& v) { glVertex4iv(v.ptr()); }
void glTexCoord2d(const KLIB::Vector2d& t) { glTexCoord2dv(t.ptr()); }
void glTexCoord2f(const KLIB::Vector2f& t) { glTexCoord2fv(t.ptr()); }
void glTexCoord3d(const KLIB::Vector3d& t) { glTexCoord3dv(t.ptr()); }
void glTexCoord3f(const KLIB::Vector3f& t) { glTexCoord3fv(t.ptr()); }
void glTexCoord4d(const KLIB::Vector4d& t) { glTexCoord4dv(t.ptr()); }
void glTexCoord4f(const KLIB::Vector4f& t) { glTexCoord4fv(t.ptr()); }
void glNormal3d(const KLIB::Vector3d& n) { glNormal3dv(n.ptr()); }
void glNormal3f(const KLIB::Vector3f& n) { glNormal3fv(n.ptr()); }

void glTranslated(const KLIB::Vector3d& t) { ::glTranslated(t.x_, t.y_, t.z_); }
void glTranslatef(const KLIB::Vector3f& t) { ::glTranslatef(t.x_, t.y_, t.z_); }

}
