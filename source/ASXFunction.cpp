#include <Nephilim/ASXFunction.h>
#include <Nephilim/ASXModule.h>
#include <Nephilim/ASXRuntime.h>
#include <AS/angelscript.h>

NEPHILIM_NS_BEGIN

ASXFunctionBase::ASXFunctionBase()
: m_function(NULL)
, m_runtime(NULL)
, m_prepared(false)
{

}


ASXFunctionBase::ASXFunctionBase(ASXModule& module, ASXRuntime& runtime, const String& function)
: m_function(NULL)
, m_runtime(NULL)
, m_prepared(false)
{
	m_runtime = &runtime;
	m_function = module.get()->GetFunctionByDecl(function.c_str());
}

void ASXFunctionBase::prepare()
{
	if(!m_prepared && m_runtime && m_function) 
	{
		m_runtime->get()->Prepare(m_function);
		m_prepared = true;
	}
}

void ASXFunctionBase::call()
{
	if(m_runtime && m_function)
	{
		m_runtime->get()->PushState();
		prepare();
		m_runtime->get()->Execute();
		m_prepared = false;
		m_runtime->get()->PopState();
	}
}

float ASXFunctionBase::getFloatReturn()
{
	return m_runtime->get()->GetReturnFloat();
}

int ASXFunctionBase::getIntReturn()
{
	return 1;
}

void* ASXFunctionBase::getObjectReturn()
{
	return NULL;
}

void ASXFunctionBase::setFloatArgument(int index, float arg)
{
	m_runtime->get()->SetArgFloat(index, arg);
}

void ASXFunctionBase::setIntArgument(int index, int arg)
{
	m_runtime->get()->SetArgDWord(index, static_cast<asDWORD>(arg));
}



NEPHILIM_NS_END