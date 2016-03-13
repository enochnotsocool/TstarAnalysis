from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'TSTARSELECT_SingleMuon_Run2015D-16Dec2015-v1_muchannel'
config.General.workArea = 'work_area'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './run_production.py'

## Input parameters
config.JobType.pyCfgParams = [
   'Mode=0',
   ]

config.Data.inputDataset = '/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD'
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 32
config.Data.outLFNDirBase = '/store/user/yichen/tstar_76X/'

config.Site.storageSite = 'T3_TW_NTU_HEP'
