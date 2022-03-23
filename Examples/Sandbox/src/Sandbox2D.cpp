#include <Odd.h>

class Sandbox2D : public Odd::Application
{
	public:
		Sandbox2D() {}
		~Sandbox2D() {}
};
 
Odd::Application* Odd::CreateApplication()
{
	return new Sandbox2D();
}