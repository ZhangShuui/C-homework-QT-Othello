#ifndef MCTS_PYQT_H
#define MCTS_PYQT_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS
#include <QVariant>
class MCTS_pyqt
{
public:
    MCTS_pyqt(const char* );
    ~MCTS_pyqt();
    bool callPyFunc(const char*,const QVariantList & args,QVariant*);
private:
    PyObject *m_pModule = nullptr;
};

#endif // MCTS_PYQT_H
