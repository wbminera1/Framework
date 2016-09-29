#if ( !defined(COMMANDENUMS_H_) || defined(GENERATE_ENUM_STRINGS) )
#define COMMANDENUMS_H_

#undef DECL_ENUM
#undef BEGIN_ENUM
#undef END_ENUM
#undef ENUMTOSTR
#define ENUMTOSTR( element ) "<" #element ">"
#ifndef GENERATE_ENUM_STRINGS
    #define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
    #define DECL_ENUM( element ) element
    #define END_ENUM( ENUM_NAME ) e##ENUM_NAME; \
            const char* ToString(enum tag##ENUM_NAME index);
#else
    #define BEGIN_ENUM( ENUM_NAME ) const char* gs_##ENUM_NAME [] =
	#define DECL_ENUM( element ) ENUMTOSTR( element )
    #define END_ENUM( ENUM_NAME ) ; const char* ToString(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }
#endif

BEGIN_ENUM(Command)
{
	DECL_ENUM(cFirst),           
	//
	DECL_ENUM(cError),           
	DECL_ENUM(cResponse),        
	DECL_ENUM(cConnect),
	DECL_ENUM(cDisconnect),
	DECL_ENUM(cExit),
	DECL_ENUM(cJSONCommand),
	DECL_ENUM(cBinaryCommand),
	//
	DECL_ENUM(cLast)
}
END_ENUM(Command)

BEGIN_ENUM(Status)
{
	DECL_ENUM(sOk),
	DECL_ENUM(sFail),
	DECL_ENUM(sWrongCommand)
}
END_ENUM(Status)


#endif /* COMMANDENUMS_H_ */
