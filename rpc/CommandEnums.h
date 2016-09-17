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
	DECL_ENUM(cFirst),           //!< cFirst
	//
	DECL_ENUM(cError),           //!< cError
	DECL_ENUM(cResponse),        //!< cResponse,
	DECL_ENUM(cPing),            //!< cPing
	DECL_ENUM(cInfo),	  	 	 //!< cInfo
	DECL_ENUM(cExit),            //!< cExit
	//
	DECL_ENUM(cLast)             //!< cLast
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
