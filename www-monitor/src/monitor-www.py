#!/usr/bin/env python3.1

import multiprocessing
import csv

import logging

from optparse import OptionParser

import shutil
import os

import time

import re

import subprocess

def getCsvValues(aFile):
    with open(aFile, 'r' ) as f:
        reader = csv.DictReader(f, delimiter=';', quotechar='"')
        for values in reader:
            yield values

def matchbasic(aDict, name):
    matcher = '[a-zA-Z0-9 -_]*'
    if not re.match(matcher, aDict[name]):
        raise ValueError('%s does not match "%s" at %s'%(key,matcher,aDict))

class Config:
    configuration_keys = ['identifier']
    

    def validate(self, aDict): 
        for key in self.configuration_keys:
            if key not in aDict:
                raise KeyError('%s not found from %s'%(key,aDict))
            
        matchbasic('identifier')

        
        return aDict

    def read_properties(self, aFile):
        properties = list(getCsvValues(aFile))
        
        properties = [self.validate(i) for i in properties if i != None ]
        
        self.properties = read_properties
        self.propertyfile = aFile
    
    
    
    def setkey(self):
        keyed = {}
        for entry in self.properties:
            if entry['identifier'] in keyed:
                raise( KeyError(entry['identifier'] + ' occurs twice'))
            keyed[entry['identifier']] = entry
        self.properties = keyed
            
    def __init__(self, aFile):
        self.propertyfile = None
        self.properties   = None
        self.read_properties(aFile)
        self.setkey()
        

    


class ApplicationConfig(Config):
    #TODO: add constants for the entry names OR USE SEARCH AND REPLACE
    configuration_keys = [
        'identifier',
        'workpath',
        'poller-processes',
        'logfile',
        'logging-level',
        'address-configuration-file'
        ]
    
    def validate(self,aDict):
        aDict = Config.validate(self, aDict)
        matchbasic('workpath')
        
        return aDict


class AddressConfig(Config):
    configuration_keys = [
        'identifier',
        'http-address',
        'polling-interval',
        'timeout',
        'validation-regexp'
        ]
    
    def validate(self, aDict): 
        aDict['polling-interval'] = int(aDict['polling-interval'])
        aDict['timeout'] = int(aDict['timeout'])
        return aDict
            

    
    def read_properties(self, aFile):
        Config.read_properties(self, aFile)
        
        def setInterval(aDict):
            return aDict
            
        self.properties = map(setInterval, self.properties)

def makearchivepath(applicationConfigDict, addressConfigDict):
    now = time.strftime("%Y-%m-%d-%H-%S")
    #TODO: better validification for pathname
    directory = applicationConfigDict['workpath'] + '/' + addressConfigDict['identifier'] + '/' + now    
    os.makedirs(directory)
    return directory

def getChildren(pid):
    p = popen('ps --no-headers -o pid --ppid %d' % pid)
    stdout, stderr = p.communicate()
    return [int(p) for p in stdout.split()]

def alarmHandler(signum, frame):
    raise Alarm

def execute(applicationConfigDict, addressConfigDict, workpath, cmd, logfile):
    
    logger = multiprocessing.get_logger()

    
    archivepath = makearchivepath(applicationConfigDict, addressConfigDict)
    
    workpath = archivepath + '/' + workpath
    os.makedirs(workpath)
    
    logger.info('cmd: '+'asdfasdf')
    
    cmd = cmd+" > %s 2>&1"%logfile
    p = subprocess.Popen(cmd, shell = True, cwd = workpath, stdout = subprocess.PIPE, stderr = subprocess.PIPE)

    signal.signal(signal.SIGALRM, alarmHandler)
    signal.alarm(addressConfigDict['timeout'])
    stdoutdata, stderrdata = [None, None]
    
    try:
        stdoutdata, stderrdata = p.communicate()
        signal.alarm(0)  # reset the alarm
        os.remove(logfile)
    except Alarm:
        print "Taking too long."
        print "output so far:"
        print open(logfile,'r').read()
        print "Killing cmd's whole process tree:"
        pids = [p.pid] + getChildren(p.pid)
        for pid in pids:
            os.kill(pid, signal.SIGKILL)
        print 'failed to execute ',cmd
        print 'restart the hw!'
        os.rename(logfile, cwd + '/cmd_execution.log')
        print 'look the file:' +cwd + '/cmd_execution.log'
        #os.remove(logfile)
        #sys.exit(1)
        raise ExecutionTimeout('timeout: %s s, command: %s'%(timeoutInSec, cmd))
    return stdoutdata, stderrdata, p.returncode

    
    pass
    

"""
TODO:
class PollingScheduler:

    def update_address_config(self, aAddrConf):
        pass
        
    def __init__(self, aAppConf):
        self.pollers = multiprocessing.Pool(aAppConf['poller-processes'])
        
"""


if __name__ == '__main__':

    parser = OptionParser()
    parser.add_option("-p", "--application-configuration-file", dest="appconffilename",
                  help="Application specific configuration. Default is %default", metavar="FILE",
                  default = "defaultconfigs/application-config.csv")
    
    parser.add_option("-n", "--application-configuration-name", dest="appconfname",
                  help="'unique name'.  Default is %default", 
                  default = "debug", type="string")
    
    
    (options, args) = parser.parse_args()

    application_config  = ApplicationConfig(options.appconffilename)
    
    appconfindex = options.appconfname
    application_config = application_config.properties[appconfindex]

    LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}

    level = LEVELS.get(application_config['logging-level'], logging.NOTSET)
    logging.basicConfig(level=level, filename = application_config['logfile'])
    
    logging.debug('-------------------------------------------------')   
    logging.debug('Application configuration read, logging enabled.')
    logging.debug('Application configuration:')
    logging.debug(application_config)
    
    address_config  = AddressConfig(application_config['address-configuration-file'])
    
    logging.debug('address configuration:')
    logging.debug(address_config.properties)
    
    
    

