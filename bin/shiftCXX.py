#!/usr/bin/python3
import json
import sys

def print_header( objectType, reps, rep, file ):
    print( "shiftCXX: Generating " + file )
    header = ""
    includes = ""
    body = ""

    header_define = "__" + rep[ "namespace" ].upper( ).replace("::", "__") + \
                        "__" + rep[ "name" ].upper( ) + "__"
    header += "#ifndef " + header_define + "\n"
    header += "#define " + header_define

    includes += "\n#include <shift/shift.h>\n"
    includes += "\n#include <Color.h>\n"

    namespaces = rep[ "namespace" ].split( "::" )
    for namespace in namespaces :
        body += "namespace " + namespace + "\n"
        body += "{\n"

    body += "  class " + rep[ "name" ] + "\n"
    if objectType == "Representation" :
        body += "    : public shift::Representation\n"
    else :
        if objectType == "Object" :
            body += "    : public shift::Object\n"
        else :
            raise Exception('objectType unknown')
    body += "  {\n"
    body += "  public:\n"

    for type in rep[ "types" ] :

        if type[ "class" ] == "enum" :
            body += "    typedef enum\n"
            body += "    {\n"
            for value in type[ "values" ] :
                body += "       " + value + ",\n"
            body += "       " + type[ "name" ].upper( ) + "_UNDEFINED\n"
            body += "    } " + type[ "name" ] + ";\n\n"

        if type[ "class" ] == "vector" :
            includes += "#include <vector>\n"
            if type[ "element" ] in reps :
                includes += "#include <shift_" + type[ "element" ] + ".h>\n"

            body += "    typedef std::vector< " + type[ "element" ] + \
                    " > " + type[ "name" ] + ";\n\n"


    #body += "    " + rep[ "name" ] + "( void );\n"

    body += "    " + rep[ "name" ] + "( \n"
    i = 1
    for prop in rep[ "properties" ] :
        body += "      " + prop[ "type" ] + " " + prop[ "name" ] + " = " + \
                prop[ "type" ] + "( )"
        if i == len( rep[ "properties" ] ) :
            body += " );\n"
        else :
            body += ",\n"
            i = i + 1

    body += "    " + rep[ "name" ] + "( const " + rep[ "name" ] + "& );\n"
    body += "    virtual ~" + rep[ "name" ] +  "( void );\n"

    body += "  };\n"
    # body += "}\n"
    for namespace in namespaces :
        body += "}\n"

    body += "#endif // " + header_define + "\n"

    # print header
    # print includes
    # print body
    f = open( file, 'w')
    f.write( header )
    f.write( includes )
    f.write( body )


def print_impl( rep, file ):
    print( "shiftCXX: Generating " + file )
    includes = ""
    body = ""

    includes += "#include <shift_" + rep[ "name" ] + ".h>\n"

    namespaces = rep[ "namespace" ].split( "::" )
    for namespace in namespaces :
        body += "namespace " + namespace + "\n"
        body += "{\n"

    body += "  " + rep[ "name" ] + "::" + rep[ "name" ] + "(\n"
    i = 1;
    for prop in rep[ "properties" ] :
        body += "      " + prop[ "type" ] + " " + prop[ "name" ]
        if i == len( rep[ "properties" ] ) :
            body += " )\n"
        else :
            body += ",\n"
            i = i + 1
    body += "  {\n"

    for prop in rep[ "properties" ] :
        body += "    this->registerProperty( \"" + prop[ "name" ] + "\", " \
                + prop[ "name" ] + " );\n"
    body += "  }\n"
    body += "  " + rep[ "name" ] + "::" + rep[ "name" ] + \
            "( const " + rep[ "name" ] + "& other )\n  {"
    body += "\n"
    for prop in rep[ "properties" ] :
        body += "    this->registerProperty(\n      \"" + prop[ "name" ] \
                + "\",\n" + "       other.getProperty( \"" + prop[ "name" ] \
                + "\" ).value< " +  prop[ "type" ] + " >( ));\n"
    body += "  }\n"
    body += "  " + rep[ "name" ] + "::~" + rep[ "name" ] +  "( void ) {}\n"

    for namespace in namespaces :
        body += "}\n"

    f = open( file, 'w')

    f.write( includes )
    f.write( body )
    # print includes
    # print body

def main(argv):
    print( "shiftCXX: Generating objects" )

    objectType = argv[0]
    repName = argv[1]
    inputfile = argv[2]
    outputdir = argv[3]

    with open( inputfile ) as data_file:

        data = json.load( data_file )

        repsNames = dict( )
        for rep in data["reps"] :
            repsNames[ rep[ "name" ]] = 1;

        for rep in data["reps"] :
            if rep[ "name" ] == repName :
                print_header( objectType, \
                              repsNames, \
                              rep, outputdir + \
                              "/shift_" + rep[ "name" ] + ".h" )
                print_impl( rep, outputdir + "/shift_" + rep[ "name" ] + \
                            ".cpp" );

if __name__ == "__main__":
    main( sys.argv[1:] )
