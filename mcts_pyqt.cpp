#include "mcts_pyqt.h"
#include <QDebug>
MCTS_pyqt::MCTS_pyqt(const char* module)
{
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C://Users//zsr//Documents//Othello//')");
    if(!Py_IsInitialized()){
        qDebug() << __FUNCTION__ << "song" << "py init fail";
        return;

    }
    this->m_pModule = PyImport_ImportModule(module);
}
MCTS_pyqt::~MCTS_pyqt(){
    Py_Finalize();
}

bool MCTS_pyqt::callPyFunc(const char * func, const QVariantList &args, QVariant *backvar){
    if(m_pModule == nullptr){
        qDebug() << __FUNCTION__ <<"func_call"<< "module is null";
        return false;
    }
    PyObject * pyFunc = PyObject_GetAttrString(m_pModule,func);
    if(!pyFunc){
        qDebug() << __FUNCTION__ << "func_call" << "load function fail";
        return false;
    }
    PyObject* pArgs =PyTuple_New(args.size());
    for (int i=0; i<args.size();++i) {
        QVariant arg = args.at(i);
        if(arg.type() ==QVariant::String){
            QString str = arg.toString();
            std::string str2 = str.toStdString();
            const char *ch = str2.c_str();
            PyTuple_SetItem(pArgs , i, Py_BuildValue("s", ch));
        }else if(arg.type() ==QVariant::Int){
            PyTuple_SetItem(pArgs, i, Py_BuildValue("i", arg.toInt()));
        }
    }
    auto pReturn = PyObject_CallObject(pyFunc,pArgs);
    if(backvar){
        if(backvar->type()==QVariant::String){
            char *s = nullptr;
            PyArg_Parse(pReturn, "s", &s);
            QString str(s);
            *backvar = QVariant::fromValue(str);
        }else if(backvar->type()==QVariant::Int){
            int nResult;
            PyArg_Parse(pReturn, "i", &nResult);
            *backvar = QVariant::fromValue(nResult);
        }
    }
    return pReturn!=nullptr;
}
