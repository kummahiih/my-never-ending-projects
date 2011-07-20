#!/usr/bin/env python3.1

import multiprocessing
import csv

import logging

from optparse import OptionParser

import shutil
import os

import time


def getCsvValues(aFile):
    with open(aFile, 'r' ) as f:
        reader = csv.DictReader(f, delimiter=';', quotechar='"')
        for values in reader:
            yield values
    
def validate_dictionary(aDict, configuration_keys):
    for key in configuration_keys:
        if key not in aDict:
            raise KeyError('%s not found from %s'%(key,aDict))


class Config:
    configuration_keys = ['identifier']

    def read_properties(self, aFile):
        read_properties = list(getCsvValues(aFile))
        
        read_properties = [i for i in read_properties if i != None ]
        
        def validate(aDict): validate_dictionary(self.configuration_keys, aDict)
        map(validate, read_properties)
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
    

                  


class AddressConfig(Config):
    configuration_keys = [
        'identifier',
        'http-address',
        'polling-interval',
        'validation-regexp'
        ]
    
    def read_properties(self, aFile):
        Config.read_properties(self, aFile)
        
        def setInterval(aDict):
            aDict['polling-interval'] = int(aDict['polling-interval'])
            return aDict
            
        self.properties = map(setInterval, self.properties)

def makearchivepath(applicationConfigDict, addressConfigDict):
    #TODO: some sort of escape / validation here
    now = time.strftime("%Y-%m-%d-%H-%S")
    directory = applicationConfigDict['workpath'] + '/' + addressConfigDict['identifier'] + '/' + now
    
    os.makedirs(directory)
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
    
    
    

