from DataFormats.FWLite import Handle, Events
import ROOT
events = Events("./edmOut.root")
jets  = Handle("vector<pat::Jet>")
label = ('skimmedPatJets')

for event in events:
   event.getByLabel( label , jets )
   print jets.product().size()
   for jet in jets.product() :
       print jet.genJet();
       print jet.genParton();
