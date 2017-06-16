#ifndef OMNI_PROPERTY_H
#define OMNI_PROPERTY_H

#define OMNI_PROPERTY_RW(TYPE,READNAME,WRITENAME) \
public:\
    TYPE READNAME() const {\
        return READNAME##_;\
    }\
    void WRITENAME(TYPE _##READNAME) {\
        if (READNAME##_ != _##READNAME) {\
            READNAME##_ = _##READNAME;\
            emit READNAME##Changed();\
        }\
    }\
private:\
    TYPE READNAME##_;\
    Q_PROPERTY(TYPE READNAME READ READNAME WRITE WRITENAME NOTIFY READNAME##Changed)

#define OMNI_PROPERTY_RW_DEFAULT(TYPE,READNAME,WRITENAME,DEFAULT) \
public:\
    TYPE READNAME() const {\
        return READNAME##_;\
    }\
    void WRITENAME(TYPE _##READNAME) {\
        if (READNAME##_ != _##READNAME) {\
            READNAME##_ = _##READNAME;\
            emit READNAME##Changed();\
        }\
    }\
private:\
    TYPE READNAME##_ = DEFAULT;\
    Q_PROPERTY(TYPE READNAME READ READNAME WRITE WRITENAME NOTIFY READNAME##Changed)

#define OMNI_PROPERTY_CLAMPED(TYPE,READNAME,WRITENAME,DEFAULT,MIN,MAX)\
public:\
    TYPE READNAME() const {\
        return READNAME##_;\
    }\
    void WRITENAME(TYPE _##READNAME) {\
        TYPE newVal = qBound(TYPE(MIN),_##READNAME,TYPE(MAX));\
        if (READNAME##_ != newVal) {\
            READNAME##_ = newVal; \
            emit READNAME##Changed();\
        }\
    }\
private:\
    TYPE READNAME##_ = DEFAULT;\
    Q_PROPERTY(TYPE READNAME READ READNAME WRITE WRITENAME NOTIFY READNAME##Changed)

#define OMNI_PROPERTY_OBJ(TYPE,READNAME) \
public:\
    TYPE* READNAME() const {\
        return READNAME##_;\
    }\
private:\
    TYPE* READNAME##_ = nullptr;\
    Q_PROPERTY(TYPE* READNAME READ READNAME CONSTANT)


#endif // OMNI_PROPERTY_H
