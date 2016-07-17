#!/usr/bin/python3
import json
import sys

def print_header( reps, rep, file ):
    print( "shiftCXX: Generating " + file )
    header = ""
    includes = ""
    body = ""

    header_define = "__" + rep[ "namespace" ].upper( ) + \
                        "__" + rep[ "name" ].upper( ) + "__"
    header += "#ifndef " + header_define + "\n"
    header += "#define " + header_define

    includes += "\n#include <shift/shift.h>\n"
    includes += "\n#include <Color.h>\n"

    body += "namespace " + rep[ "namespace" ] + "\n"
    body += "{\n"
    body += "namespace shiftgen\n"
    body += "{\n"
    body += "  class " + rep[ "name" ] + "\n"
    body += "    : public shift::Representation\n"
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


    body += "    " + rep[ "name" ] + "( void );\n"
    body += "    " + rep[ "name" ] + "( const " + rep[ "name" ] + "& );\n"
    body += "    virtual ~" + rep[ "name" ] +  "( void );\n"

    body += "  };\n"
    body += "}\n"
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
    body += "namespace " + rep[ "namespace" ] + "\n"
    body += "{\n"
    body += "namespace shiftgen\n"
    body += "{\n"
    body += "  " + rep[ "name" ] + "::" + rep[ "name" ] + "( void )\n  {"
    body += "\n"
    for prop in rep[ "properties" ] :
        body += "    this->registerProperty( \"" + prop[ "name" ] + "\", " \
                + prop[ "type" ] + "( ));\n"
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

    body += "}\n"
    body += "}\n"

    f = open( file, 'w')

    f.write( includes )
    f.write( body )
    # print includes
    # print body

def main(argv):
    print( "shiftCXX: Generating objects" )
    repName = argv[0]
    inputfile = argv[1]
    outputdir = argv[2]
    with open( inputfile ) as data_file:

        data = json.load( data_file )

        repsNames = dict( )
        for rep in data["reps"] :
            repsNames[ rep[ "name" ]] = 1;

        for rep in data["reps"] :
            if rep[ "name" ] == repName :
                print_header( repsNames,
                              rep, outputdir + \
                              "/shift_" + rep[ "name" ] + ".h" )
                print_impl( rep, outputdir + "/shift_" + rep[ "name" ] + \
                            ".cpp" );

if __name__ == "__main__":
   main( sys.argv[1:] )
