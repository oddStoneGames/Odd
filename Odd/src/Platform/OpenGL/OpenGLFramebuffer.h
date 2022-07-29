#pragma once
#include "Odd/Renderer/FrameBuffer.h"

namespace Odd
{
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(const uint32_t width, const uint32_t height) override;
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();
	private:
		static const uint32_t s_MaxFramebufferSize = 15360;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};
}