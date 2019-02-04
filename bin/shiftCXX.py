#!/usr/bin/python3
import json
import sys

def findPropAttrib( ents, entName, propName, attribName ) :
    for ent in ents :
        if ent[ "name" ] == entName :
            for prop in ent[ "properties" ] :
                if prop[ "name" ] == propName and attribName in prop :
                    return prop[ attribName ]
    raise Exception('ERROR: findPropAttrib failed')

def print_header( objectType, ents, ent, file ):
    print( "shiftCXX: Generating " + file )
    header = ""
    includes = ""
    body = ""

    header_define = "__" + ent[ "namespace" ].upper( ).replace("::", "__") + \
                        "__" + ent[ "name" ].upper( ) + "__"
    header += "#ifndef " + header_define + "\n"
    header += "#define " + header_define

    includes += "\n\n#include <shift/shift.h>\n"
    includes += "\n"

    if "includes" in ent:
        customIncludes = ent[ "includes" ]
        for include in customIncludes :
            includes += "#include " + include + "\n"

    # Namespaces
    namespaces = ent[ "namespace" ].split( "::" )
    for namespace in namespaces :
        body += "namespace " + namespace + "\n"
        body += "{\n"

    # Class
    body += "  class " + ent[ "name" ] + "\n"
    if objectType == "Representation" :
        body += "    : public shift::Representation\n"
    else :
        if objectType == "Relationship" :
            body += "    : public shift::RelationshipProperties\n"
        else :
            if objectType == "Entity" :
                body += "    : public shift::Entity\n"
            else :
                raise Exception('objectType unknown')
    body += "  {\n"

    body += "  protected:\n" \
            "  static const std::string _typeName;\n"

    body += "  public:\n"

    # Types
    for type in ent[ "types" ] :

        if type[ "class" ] == "enum" :
            body += "    typedef enum\n"
            body += "    {\n"
            for value in type[ "values" ] :
                body += "       " + value.replace( " ", "_" ).replace( "-", "_" ) + ",\n"
            body += "       " + type[ "name" ].upper( ) + "_UNDEFINED\n"
            body += "    } " + type[ "name" ] + ";\n\n"

        if type[ "class" ] == "vector" :
            includes += "#include <vector>\n"
            if type[ "element" ].replace("*", "") in ents :
                includes += "#include <shift_" +\
                type[ "element" ].replace("*", "") + ".h>\n"

            body += "    typedef std::vector< " + type[ "element" ] + \
                    " > " + type[ "name" ] + ";\n\n"

        if type[ "class" ].endswith( "map" ) :
            typename = str( type[ "class" ])
            includes += "#include <" + typename.replace( "multi", "") + ">\n"
            body += "    typedef std::" + typename + "< " + type[ "key" ] + \
                    ", " + type[ "element" ] + " > " + type[ "name" ] + ";\n\n"
            body += "    typedef std::pair< " + type[ "name"] + "::const_iterator, \n" + \
                    "                       " + type[ "name"] + "::const_iterator>" + \
                    "T" + ent[ "name"] + "Range;\n\n"

    # Constructor
    body += "    " + ent[ "name" ] + "(\n"
    i = 1
    if "properties" in ent and len( ent[ "properties"]) > 0:
        for prop in ent[ "properties" ] :
            if prop[ "type" ] in ents :
                includes += "#include <shift_" + prop[ "type" ] + ".h>\n"
    
            body += "      " + prop[ "type" ] + " " + \
                    prop[ "name" ].replace(" ", "") + " = " + \
                    prop[ "type" ] + "( )"
            if i == len( ent[ "properties" ] ) :
                body += "\n    );\n"
            else :
                body += ",\n"
                i = i + 1
    else:
        body += " );\n"

    # Copy constructor declaration
    body += "    " + ent[ "name" ] + "( const " + ent[ "name" ] + "& );\n"
    # Destructor declaration
    body += "    virtual ~" + ent[ "name" ] +  "( void );\n"

    # Type Name method declaration and definition
    if objectType == "Entity" or objectType == "Relationship":
        body += "    const std::string& typeName( void ) const final {"\
                " return _typeName; }\n"

    # create method for entities declaration
    if objectType == "Entity" :
        body += "    virtual shift::Entity* create( void ) const override;\n"

    # create subentities method for entities declaration
    if objectType == "Entity" :
        body += "    virtual void createSubEntities(\n" + \
                "      std::vector< shift::Entity* >& subEntities ) const override;\n"

    # create method for relationships declaration
    if objectType == "Relationship" :
        body += "    virtual shift::RelationshipProperties* create( void ) const override;\n"

    # issubentity method declaration and definition
    if objectType == "Entity" and "subentity" in ent[ "flags" ] :
        body += "    inline virtual bool isSubEntity( void ) final { return true; }\n"

    # isNotHierarchy method declaration and definition
    if objectType == "Entity" and "noHierarchy" in ent[ "flags" ] :
        body += "    inline virtual bool isNotHierarchy( void ) final { return true; }\n"

    body += "    virtual bool evalConstraint(\n" +\
            "      const shift::Properties::PropertyConstraintType& constraintType,\n" + \
            "      const std::string& propertyName ) const final;\n"

    # autoUpdateProperties method declaration
    if objectType == "Entity" or objectType == "Relationship":
        body += "    void autoUpdateProperties( void ) final;\n";

    # autoUpdateProperty method declaration
    if objectType == "Entity" or objectType == "Relationship":
        body += "    void autoUpdateProperty( fires::Object* /* obj */,\n" + \
                "                             const std::string& /* propertyLabel */ ) final;\n";

    # set related dependencies method declaration
    if objectType == "Entity" :
        body += "    void setRelatedDependencies( const std::string& relName,\n" \
                "                                 shift::Entity* dependency ) final;\n"
    # remove related dependencies method declaration
    if objectType == "Entity" :
        body += "    void removeRelatedDependencies( const std::string& relName,\n" \
                "                                 shift::Entity* dependency ) final;\n"

    # set related dependencies method declaration
    if objectType == "Relationship" :
        body += "    void setRelatedDependencies( shift::RelationshipProperties* dependency ) final;\n"

    # remove related dependencies method declaration
    if objectType == "Relationship" :
        body += "    void removeRelatedDependencies( shift::RelationshipProperties* dependency ) final;\n"

    # propertyFlags method declaration and definition
    if objectType == "Entity" :
        body += "    inline virtual bool hasPropertyFlag( const std::string& propertyLabel,\n" + \
                "                                         shift::Entity::TPropertyFlag flag ) const final\n" +\
                "    {\n" + \
                "      return ( " + ent[ "name"] + "::_propertyFlags[ propertyLabel ].count( flag ) > 0 );\n" + \
                "    }\n\n"

        # propertyFlags static map
        body += "  protected:\n" + \
                "    static shift::Entity::TPropertiesFlagsMap _propertyFlags;\n"

    # propertyFlags method declaration and definition
    if objectType == "Relationship" :
        body += "    inline virtual bool hasPropertyFlag( const std::string& propertyLabel,\n" + \
                "                                         shift::RelationshipProperties::TPropertyFlag flag ) const final\n" + \
                "    {\n" + \
                "      return ( " + ent[ "name"] + "::_propertyFlags[ propertyLabel ].count( flag ) > 0 );\n" + \
                "    }\n\n"

        # propertyFlags static map
        body += "  protected:\n" + \
                "    static shift::RelationshipProperties::TPropertiesFlagsMap _propertyFlags;\n"

    # end of class
    body += "  };\n"
    # body += "}\n"

    for namespace in namespaces :
        body += "}\n"

    body += "#endif // " + header_define + "\n"

    # print header
    # print includes
    # print body
    f = open( file, 'w')
    f.write( "// File generated by shiftCXX.py. Do not edit.\n" )
    f.write( header )
    f.write( includes )
    f.write( body )


def print_impl( objectType, ents, ent, file ):
    print( "shiftCXX: Generating " + file )
    includes = ""
    body = ""

    includes += "#include <shift_" + ent[ "name" ] + ".h>\n"

    # Namespaces
    namespaces = ent[ "namespace" ].split( "::" )
    for namespace in namespaces :
        body += "namespace " + namespace + "\n"
        body += "{\n"

    body += "  const std::string " + ent[ "name" ] + \
            "::_typeName = \"" + ent[ "name" ] + "\";\n\n"

    # property flags map initialization for entities
    if objectType == "Entity" :
        body += "  shift::Entity::TPropertiesFlagsMap " + ent[ "name" ] + "::_propertyFlags =\n" + \
                "  {\n"

        for prop in ent[ "properties" ] :
            body += "  { \"" + prop[ "name" ] + "\",\n    {\n"
            if "flags" in prop :
                for flag in prop[ "flags" ] :
                    body += "       shift::Entity::TPropertyFlag::" + flag;
                    if prop[ "flags"].index( flag ) != len( prop[ "flags"] ) - 1 :
                        body += ","
                    body += "\n"
            body += "    }\n"
            body += "  }"
            if ent[ "properties"].index( prop ) != len( ent[ "properties" ] ) - 1 :
                body += ","
            body += "\n"
        body += "  };\n\n"

        # property flags map initialization for relationships
    if objectType == "Relationship" :
        body += "  shift::RelationshipProperties::TPropertiesFlagsMap " + ent[ "name" ] + "::_propertyFlags =\n" + \
                "  {\n"

        for prop in ent[ "properties" ] :
            body += "  { \"" + prop[ "name" ] + "\",\n    {\n"
            if "flags" in prop :
                for flag in prop[ "flags" ] :
                    body += "       shift::RelationshipProperties::TPropertyFlag::" + flag;
                    if prop[ "flags"].index( flag ) != len( prop[ "flags"] ) - 1 :
                        body += ","
                    body += "\n"
            body += "    }\n"
            body += "  }"
            if ent[ "properties"].index( prop ) != len( ent[ "properties" ] ) - 1 :
                body += ","
            body += "\n"
        body += "  };\n\n"

    # Constructor definition
    body += "  " + ent[ "name" ] + "::" + ent[ "name" ] + "(\n"
    i = 1;
    if "properties" in ent and len( ent["properties" ]) > 0:
        for prop in ent[ "properties" ] :
            body += "      " + prop[ "type" ] + " " + \
                    prop[ "name" ].replace(" ", "") + "__"
            if i == len( ent[ "properties" ] ) :
                body += " )\n"
            else :
                body += ",\n"
                i = i + 1
    else:
        body += " void )\n"

    body += "  {\n"

    types = ent[ "types" ]

    for prop in ent[ "properties" ] :

        type = {}
        for t in types :
            if t[ "name" ] == prop[ "type" ] :
                type = t
                break;
        enumValues = ""
        if type != {} and type[ "name" ] == prop[ "type" ] and  type[ "class" ] == "enum" :
            enumValues = ",\n      {"
            values = type[ "values" ]
            for value in values :
                enumValues += "\n        { " + value.replace( " ", "_" ).replace( "-", "_" ) +\
                              ", \"" + value + "\" }"
                if values.index( value ) != len( values ) - 1 :
                    enumValues += ", "
            enumValues += "\n      }"

        body += "    fires::PropertyManager::registerProperty<" +\
                prop[ "type" ] +\
                ">( this, \"" +\
                prop[ "name" ] + "\", " \
                + prop[ "name" ].replace(" ", "") + "__" + enumValues + " );\n"
    body += "  }\n"

    # Copy constructor definition
    body += "  " + ent[ "name" ] + "::" + ent[ "name" ] + \
            "( const " + ent[ "name" ] + "& "+ \
            ( "other" if len( ent["properties" ]) > 0 else "/*other*/" ) + \
            ")\n  {"

    body += "\n"
    for prop in ent[ "properties" ] :
        body += "    this->registerProperty(\n      \"" + prop[ "name" ] \
                + "\",\n" + "       other.getProperty( \"" + prop[ "name" ] \
                + "\" ).value< " +  prop[ "type" ] + " >( ));\n"
    body += "  }\n"
    body += "  " + ent[ "name" ] + "::~" + ent[ "name" ] +  "( void ) {}\n"

    # create method definition
    if objectType == "Entity" :
        body += "  shift::Entity* " + ent[ "name" ] + "::create( void ) const\n"
        body += "  {\n"
        body += "    return new " + ent[ "name" ] + "( *this );\n"
        body += "  }\n"

    # create subentities method definition
    if objectType == "Entity" :
        body += "  void " + ent[ "name" ] + \
                "::createSubEntities(\n" + \
                "    std::vector< shift::Entity* >& subentities ) const\n"
        body += "  {\n    (void) subentities;\n"
        nbEntitiesCreated = 0
        subEntCode = ""
        countEntitiesCode = "    unsigned int nbEntities = 0;\n"
        if "subentities" in ent:
            for subentity in ent[ "subentities" ] :
                includes += "#include <shift_" + subentity[ "name" ] + ".h>\n"

                closeLoop = False;
                if "repeat" in subentity:
                    if  subentity[ "repeat" ][ "type" ] == "range" :
                        loopCode = "    for ( auto " + \
                                   subentity[ "repeat" ][ "name" ] + " = " + \
                                   subentity[ "repeat" ][ "init" ] + "; " + \
                                   subentity[ "repeat" ][ "name" ] + " <= " + \
                                   subentity[ "repeat" ][ "end" ] + "; " + \
                                   subentity[ "repeat" ][ "name" ] + " += " + \
                                   subentity[ "repeat" ][ "inc" ] + " )\n    "
                        closeLoop = True;
                        subEntCode += loopCode + "{\n"
                        countEntitiesCode += loopCode + "  ++nbEntities;\n"
                        # print( len( range( int( subentity[ "repeat" ][ "init" ] ),
                        #               int( subentity[ "repeat" ][ "end" ] ),
                        #               int( subentity[ "repeat" ][ "inc" ] ))))
                        # nbEntitiesCreated += len( range( int( subentity[ "repeat" ][ "init" ] ),
                        #                                  int( subentity[ "repeat" ][ "end" ] ),
                        #                                  int( subentity[ "repeat" ][ "inc" ] )))

                subEntCode += "      shift::Entity* entity = new " + subentity[ "name" ] + ";\n"
                subEntCode += "      subentities.emplace_back( entity );\n"
                if "properties" in subentity :
                    for prop in subentity[ "properties" ] :
                        if prop[ "type" ] == "linked" :
                            subEntCode += "      entity->registerProperty(\n        \"" + \
                                          prop[ "property" ] + "\",\n         ( " + \
                                          findPropAttrib( ents, subentity[ "name" ], \
                                                    prop[ "property" ], "type" ) + " )"
                            subEntCode += " this->getProperty( \"" + \
                                          prop[ "origin" ] + "\" ).value< " +\
                                          findPropAttrib( ents, ent[ "name" ], \
                                                    prop[ "origin" ], "type" ) + \
                                    " >( ));\n"
                        if prop[ "type" ] == "fixed" :
                            subEntCode += "      entity->registerProperty(\n        \"" + \
                                          prop[ "property" ] + "\",\n         " + \
                                          prop[ "value" ] + " );\n"

                if closeLoop :
                    subEntCode += "    }\n"
                # subEntCode += "}"
                # subEntCode += subentity[ "name" ]
            body += countEntitiesCode
            body += "    subentities.reserve( subentities.size( ) + nbEntities );\n\n"
            body += subEntCode

        body += "  }\n"

        # if nbEntitiesCreated > 0 :
        #     body += "subentities.reserve( subentities.size( ) + " + \
        #             str( nbEntitiesCreated ) + " );\n"

    #create method definition for Relationships
    if objectType == "Relationship" :
        body += "  shift::RelationshipProperties* " + ent[ "name" ] + "::create( void ) const\n"
        body += "  {\n"
        body += "    return new " + ent[ "name" ] + "( *this );\n"
        body += "  }\n"


    body += "    bool " + ent[ "name" ] + "::evalConstraint(\n" +\
            "      const shift::Properties::PropertyConstraintType& constraintType,\n" + \
            "      const std::string& propertyName ) const\n"
    body += "  {\n"
    body += "    (void) constraintType; (void) propertyName;\n"
    body += "    if ( constraintType == shift::Properties::SUBPROPERTY )\n    {\n"

    for prop in ent[ "properties" ] :
        if "constraints" in prop :
            body += "      if ( propertyName == \"" + prop[ "name" ] + "\" )\n"
            body += "      {\n"
            for constraint in prop[ "constraints" ] :
                body += "        bool ret = true;\n"
                if constraint[ "type" ] == "subproperty" :
                    body += "        ret = ret && ( this->getProperty( \"" + \
                            constraint[ "parentProperty" ] + "\").value< " + \
                            findPropAttrib( ents, ent[ "name" ], \
                                            constraint[ "parentProperty" ], "type" ) + \
                            " >( ) == " + constraint[ "parentValue" ].replace( " ", "_" ).replace( "-", "_" ) + " );\n"
            body += "      return ret;\n"
            body += "      }\n"
    body += "    }\n"
    body += "    return true;\n"
    body += "  }\n"

    # autoUpdateProperties method definition
    if objectType == "Entity" or objectType == "Relationship":
        body += "  void " + ent[ "name" ] + "::autoUpdateProperties( )\n" + \
                "  {\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop :
                auto = prop[ "auto" ]
                if ( auto[ "op" ] == "SUM" or auto[ "op" ] == "MEAN" or \
                     auto[ "op" ] == "MAX" or auto[ "op" ] == "MIN" or \
                     auto[ "op" ] == "COUNT" ) and \
                        "source" in auto and "entities" in auto[ "source" ]:
                    body +=  "    this->autoUpdateProperty(nullptr, \"" + prop[ "name" ]+"\" );\n"
        body += "  }\n"

    # autoUpdateProperty method definition for entities
    if objectType == "Entity":
        body += "  void " + ent[ "name" ] + "::autoUpdateProperty(\n" + \
                "    fires::Object* /* obj */,\n" + \
                "    const std::string& propertyLabel )\n" + \
                "  {\n" \
                "    ( void ) propertyLabel;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop :
                auto = prop[ "auto" ]
                if ( auto[ "op" ] == "SUM" or auto[ "op" ] == "MEAN" or \
                     auto[ "op" ] == "MAX" or auto[ "op" ] == "MIN" or \
                     auto[ "op" ] == "COUNT" ) and \
                     "source" in auto and "entities" in auto[ "source" ]:
                    body +=  "    if ( propertyLabel == \"" + prop[ "name" ] + "\" )\n"
                    body += \
                    "      this->autoUpdatePropertyWithRelatedEntities(" + \
                    "\n        \"" + auto[ "source" ][ "relName" ] + "\"" \
                    ",\n        { ";
                    body += ', '.join( '"' + str( el ) + '"' for el in auto[ "source" ][ "entities" ] )
                    # for ent in auto[ "entities" ] :
                    #     body += ent + ","
                    if auto[ "op" ] == "SUM" : op = "TAutoUpdatePropertyOp::SUM"
                    elif auto[ "op" ] == "MEAN" : op = "TAutoUpdatePropertyOp::MEAN"
                    elif auto[ "op" ] == "MAX" : op = "TAutoUpdatePropertyOp::MAX"
                    elif auto[ "op" ] == "MIN" : op = "TAutoUpdatePropertyOp::MIN"
                    elif auto[ "op" ] == "COUNT" : op = "TAutoUpdatePropertyOp::COUNT"
                    if "property" in auto[ "source" ] :
                        origPropName = auto[ "source" ][ "property" ]
                    else :
                        origPropName = ""
                    body += " }" +\
                    ",\n        " + op +", \"" + \
                    origPropName  + "\", \"" + \
                    prop[ "name" ] + "\" );\n"

        body += "  }\n"

    # set related dependencies method definition for entities
    if objectType == "Entity" :
        body += "  void " + ent[ "name" ] + "::setRelatedDependencies(\n" \
                "    const std::string& relName,\n" \
                "    shift::Entity* dependency )\n" \
                "  {\n    ( void ) relName; ( void ) dependency;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop and \
               "source" in prop[ "auto" ] and \
               "entities" in prop[ "auto" ][ "source" ] and \
               "property" in prop[ "auto" ][ "source" ] :
                auto = prop[ "auto" ]
                relEntComp = [ "dependency->typeName( ) == \"" + s + "\""  for s in auto[ "source" ][ "entities" ]]
                body += "    if ( relName == \"" + auto[ "source" ][ "relName" ] +"\" &&\n" + \
                        "        ( "
                body += ' ||\n          '.join( str( relEnt ) for relEnt in relEntComp )
                body += " ))\n" + \
                        "    {\n"
                body += "      fires::DependenciesManager::addDependency(\n" + \
                        "        this, \"" + \
                        prop[ "name" ] + "\", dependency, \"" + \
                        auto[ "source" ][ "property" ] + "\" );\n" + \
                        "      fires::DependenciesManager::setUpdater(\n" + \
                        "        this, \"" + prop[ "name" ] + "\", " \
                        " this, &" + ent[ "name" ] + "::autoUpdateProperty );\n" \
                        "      fires::DependenciesManager::setDependentsDirty(\n" \
                        "        this, \"" + prop[ "name" ] + "\", true );\n"


                body += "    }\n"

        body += "    }\n"

    # remove related dependencies method definition for entities
    if objectType == "Entity" :
        body += "  void " + ent[ "name" ] + "::removeRelatedDependencies(\n" \
               "    const std::string& relName,\n" \
               "    shift::Entity* dependency )\n" \
               "  {\n    ( void ) relName; ( void ) dependency;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop and \
               "source" in prop[ "auto" ] and \
               "entities" in prop[ "auto" ][ "source" ] and \
               "property" in prop[ "auto" ][ "source" ] :
                auto = prop[ "auto" ]
                relEntComp = [ "dependency->typeName( ) == \"" + s + "\""  for s in auto[ "source" ][ "entities" ]]
                body += "    if ( relName == \"" + auto[ "source" ][ "relName" ] +"\" &&\n" + \
                        "        ( "
                body += ' ||\n          '.join( str( relEnt ) for relEnt in relEntComp )
                body += " ))\n" + \
                        "    {\n"
                body += "      fires::DependenciesManager::removeDependency(\n" + \
                        "        this, \"" + \
                        prop[ "name" ] + "\", dependency, \"" + \
                        auto[ "source" ][ "property" ] + "\" );\n" + \
                        "      fires::DependenciesManager::setDependentsDirty(\n" \
                        "        this, \"" + prop[ "name" ] + "\", true );\n"


                body += "    }\n"

        body += "    }\n"

    # autoUpdateProperty method definition for Relationship
    if objectType == "Relationship":
        body += "  void " + ent[ "name" ] + "::autoUpdateProperty(\n" + \
                "    fires::Object* /* obj */,\n" + \
                "    const std::string& propertyLabel )\n" + \
                "  {\n" \
                "    ( void ) propertyLabel;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop :
                auto = prop[ "auto" ]
                if ( auto[ "op" ] == "SUM" or auto[ "op" ] == "MEAN" or \
                     auto[ "op" ] == "MAX" or auto[ "op" ] == "MIN" or \
                     auto[ "op" ] == "COUNT" ) and \
                     "source" in auto and "entities" in auto[ "source" ]:
                    body +=  "    if ( propertyLabel == \"" + prop[ "name" ] + "\" )\n"
                    body += \
                    "      this->autoUpdatePropertyWithRelatedRelations(" + \
                    "\n{ ";
                    body += ', '.join( '"' + str( el ) + '"' for el in auto[ "source" ][ "entities" ] )
                    # for ent in auto[ "entities" ] :
                    #     body += ent + ","
                    if auto[ "op" ] == "SUM" : op = "TAutoUpdatePropertyOp::SUM"
                    elif auto[ "op" ] == "MEAN" : op = "TAutoUpdatePropertyOp::MEAN"
                    elif auto[ "op" ] == "MAX" : op = "TAutoUpdatePropertyOp::MAX"
                    elif auto[ "op" ] == "MIN" : op = "TAutoUpdatePropertyOp::MIN"
                    elif auto[ "op" ] == "COUNT" : op = "TAutoUpdatePropertyOp::COUNT"
                    if "property" in auto[ "source" ] :
                        origPropName = auto[ "source" ][ "property" ]
                    else :
                        origPropName = ""
                    body += " }" +\
                    ",\n        " + op +", \"" + \
                    origPropName  + "\", \"" + \
                    prop[ "name" ] + "\" );\n"

        body += "  }\n"

    # set related dependencies method definition for relationships
    if objectType == "Relationship" :
        body += "  void " + ent[ "name" ] + "::setRelatedDependencies(\n" \
                "    shift::RelationshipProperties* dependency )\n" \
                "  {\n    ( void ) dependency;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop and \
               "source" in prop[ "auto" ] and \
               "entities" in prop[ "auto" ][ "source" ] and \
               "property" in prop[ "auto" ][ "source" ] :
                auto = prop[ "auto" ]
                relEntComp = [ "dependency->typeName( ) == \"" + s + "\""  for s in auto[ "source" ][ "entities" ]]
                body += "    if ( \n" + \
                        "        ( "
                body += ' ||\n          '.join( str( relEnt ) for relEnt in relEntComp )
                body += " ))\n" + \
                        "    {\n"
                body += "      fires::DependenciesManager::addDependency(\n" + \
                        "        this, \"" + \
                        prop[ "name" ] + "\", dependency, \"" + \
                        auto[ "source" ][ "property" ] + "\" );\n" + \
                        "      fires::DependenciesManager::setUpdater(\n" + \
                        "        this, \"" + prop[ "name" ] + "\", " \
                        " this, &" + ent[ "name" ] + "::autoUpdateProperty );\n" \
                        "      fires::DependenciesManager::setDependentsDirty(\n" \
                        "        this, \"" + prop[ "name" ] + "\", true );\n"


                body += "    }\n"

        body += "    }\n"

    # remove related dependencies method definition for relationships
    if objectType == "Relationship" :
        body += "  void " + ent[ "name" ] + "::removeRelatedDependencies(\n" \
                "    shift::RelationshipProperties* dependency )\n" \
                "  {\n    ( void ) dependency;\n"
        for prop in ent[ "properties" ] :
            if "auto" in prop and \
               "source" in prop[ "auto" ] and \
               "entities" in prop[ "auto" ][ "source" ] and \
               "property" in prop[ "auto" ][ "source" ] :
                auto = prop[ "auto" ]
                relEntComp = [ "dependency->typeName( ) == \"" + s + "\""  for s in auto[ "source" ][ "entities" ]]
                body += "    if ( \n" + \
                        "        ( "
                body += ' ||\n          '.join( str( relEnt ) for relEnt in relEntComp )
                body += " ))\n" + \
                        "    {\n"
                body += "      fires::DependenciesManager::removeDependency(\n" + \
                        "        this, \"" + \
                        prop[ "name" ] + "\", dependency, \"" + \
                        auto[ "source" ][ "property" ] + "\" );\n" + \
                        "      fires::DependenciesManager::setDependentsDirty(\n" \
                        "        this, \"" + prop[ "name" ] + "\", true );\n"


                body += "    }\n"

        body += "    }\n"

    for namespace in namespaces :
        body += "}\n"

    f = open( file, 'w')

    f.write( "// File generated by shiftCXX.py. Do not edit.\n" )
    f.write( includes )
    f.write( body )

def generateDomain( name ) :
    return ""

def main( argv ) :

    objectType = argv[0]
    inputfile = argv[1]
    outputdir = argv[2]
    domainFile = argv[3]
    entName = argv[4:]

    with open( inputfile ) as data_file:

        data = json.load( data_file )

        implFile = str( domainFile ).replace( ".h", ".cpp" )

        domainContent = "// File generated by shiftCXX.py. Do not edit.\n"

        implContent = domainContent
        implContent += "#include <" + domainFile[ str( domainFile ).rfind("/") + 1 : ] + ">\n\n"

        for ent in data["entities"] :
            if ent[ "name" ] in entName :
                domainContent += "#include <" + outputdir + "/shift_" + ent[ "name" ] + ".h>\n"

        header_define = "__" + data[ "namespace" ].upper( ).replace("::", "__") + \
                        "__" + data[ "name" ].upper( ) + "__"

        if objectType == "Entity" :
            header_define += "ENTITIES__"
        elif objectType == "Relationship" :
            header_define += "RELATIONSHIPS__"
        domainContent += "#ifndef " + header_define + "\n"
        domainContent += "#define " + header_define + "\n"

        # Namespaces
        namespaces = data[ "namespace" ].split( "::" )
        for namespace in namespaces :
            content = "namespace " + namespace + "\n{\n"
            domainContent += content
            if objectType == "Relationship":
                implContent += content

        if objectType == "Entity" :
            domainContent += "class EntitiesTypes : public shift::EntitiesTypes\n" \
                             "{\n" \
                             "public:\n" \
                             "  EntitiesTypes( void )\n" \
                             "  {\n"
            for ent in data["entities"] :
                if ent[ "name" ] in entName :
                    domainContent += "    this->_entitiesTypes.push_back( std::make_tuple( \""  + \
                                     ent[ "name" ] + "\", new " + \
                                     ent[ "namespace" ] + "::" + ent[ "name" ] + ", " + \
                                     str( "subentity" in ent[ "flags" ] ).lower( ) + ", " + \
                                     str( "noHierarchy" in ent[ "flags" ] ).lower( ) + " ));\n"
                    if "noHierarchy" in ent[ "flags" ] :
                        domainContent += "    this->_notHierarchyTypes.push_back( std::make_tuple( \""  + \
                                         ent[ "name" ] + "\", new " + \
                                         ent[ "namespace" ] + "::" + ent[ "name" ] + ", " + \
                                         str( "subentity" in ent[ "flags" ] ).lower( ) + ", " + \
                                         str( "noHierarchy" in ent[ "flags" ] ).lower( ) + " ));\n"
            domainContent += "  }\n" \
                             "  virtual ~EntitiesTypes( void )\n" \
                             "  {}\n" \
                             "};\n"

        if objectType == "Relationship" :

            # Constraints
            constraints = dict( )
            for ent in data[ "entities" ]:
                if "constraints" in ent:
                    cvalue = [ ]
                    for constraint in ent[ "constraints" ]:
                        cvalue.append( [ constraint[ "srcEntity" ], constraint[ "dstEntity" ]] )
                    constraints[ ent["name"]] = cvalue


            domainContent += "class RelationshipPropertiesTypes : public shift::RelationshipPropertiesTypes\n" + \
                             "{\n" + \
                             "public:\n\n" \
                             "  RelationshipPropertiesTypes( void );\n" \
                             "  virtual ~RelationshipPropertiesTypes( void )\n" \
                             "  {}\n\n};\n"

            # RelationshipPropertiesTypes.cpp
            scope = "RelationshipPropertiesTypes::"

            # Container initialization
            implContent += "  RelationshipPropertiesTypes::RelationshipPropertiesTypes( void )\n" \
                             "  {\n"
            for ent in data["entities"] :
                if ent["name"] in entName and "relationship" in ent:
                    implContent += "    this->_relationshipPropertiesTypes[\"" + \
                                     ent[ "relationship" ] + "\"] = new " + \
                                     ent[ "namespace" ] + "::" + ent ["name" ] + \
                                     " ;\n"
            for relationship, constraint in constraints.items( ):
                for srcEntity, dstEntity in constraint:
                    implContent += '    addConstraint("' + relationship + '", "' + srcEntity + '", "' + dstEntity + '" );\n'
            implContent += "  }\n"

        namespaces = data[ "namespace" ].split( "::" )
        for namespace in namespaces :
            domainContent += "}\n"
            if objectType == "Relationship":
                implContent += "}\n"

        domainContent += "#endif\n"


        f = open( domainFile, 'w')
        f.write( domainContent )
        f.close( )

        if objectType == "Relationship":
            f = open( implFile, 'w' )
            f.write( implContent )
            f.close( )

        entsNames = dict( )
        for ent in data["entities"] :
            entsNames[ ent[ "name" ]] = 1;

        for ent in data["entities"] :
            if ent[ "name" ] in entName :
                print_header( objectType, \
                              entsNames, \
                              ent, outputdir + \
                              "/shift_" + ent[ "name" ] + ".h" )
                print_impl( objectType, \
                            data["entities"], ent, outputdir + "/shift_" + ent[ "name" ] + \
                            ".cpp" );

if __name__ == "__main__":
    main( sys.argv[1:] )
