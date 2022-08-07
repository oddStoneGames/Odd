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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearColorAttachment(uint32_t attachmentIndex, int value) override;
		virtual void Resize(const uint32_t width, const uint32_t height) override;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override { return (index < m_ColorAttachments.size()) ? m_ColorAttachments[index] : 0; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();
	private:
		static const uint32_t s_MaxFramebufferSize = 15360;
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<uint32_t> m_ColorAttachments;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		
		uint32_t m_DepthAttachment = 0;
		FramebufferTextureSpecification m_DepthAttachmentSpecs = FramebufferTextureFormat::None;
	};
}