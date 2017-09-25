#!/usr/bin/env python

import xml.etree.ElementTree as ET
import sys

tree = ET.parse('gl.xml')
root = tree.getroot()

sys.stdout.write('// This file was automatically generated from gl.xml by createDebugHelper.py\n')
sys.stdout.write('// gl.xml was taken from https://cvs.khronos.org/svn/repos/ogl/trunk/doc/registry/public/api/\n')
sys.stdout.write('\n')
sys.stdout.write('#define wrapGlFuncNoArgs(glFunc) do { glFunc(); SG::GL::checkGlErrors(__FILE__, __LINE__); } while(0)\n')
sys.stdout.write('#define wrapGlFunc(glFunc, ...) do { glFunc(__VA_ARGS__); SG::GL::checkGlErrors(__FILE__, __LINE__); } while(0)\n')
sys.stdout.write('\n')

skipFunctions = [ "glGetBooleanv", "glGetIntegerv", "glGetFloatv", "glGetDoublev" ]

for commands in root.findall('commands'):
    for cmd in commands.findall('command'):
        proto = cmd.find('proto')
        
        returnType = proto.find('ptype')
        if returnType is not None:
            continue
        
        replacedFunc = proto.find('name').text
        if replacedFunc in skipFunctions:
            continue
        
        parameters = ''
        
        parametersList = cmd.findall('param')
        parametersCount = len(parametersList)
        if parametersCount > 0:
            i = 0
            for param in parametersList:
                paramName = param.find('name')
                parameters += paramName.text
                if i < parametersCount - 1:
                    parameters += ', '
                i += 1
        
            wrapperFunc = 'wrapGlFunc'
            wrapperParameters = replacedFunc + ', ' + parameters
        else:
            wrapperFunc = 'wrapGlFuncNoArgs'
            wrapperParameters = replacedFunc

        sys.stdout.write('#define ' + replacedFunc + '(' + parameters + ') ' + wrapperFunc + '(' + wrapperParameters + ')\n')

sys.stdout.flush()
