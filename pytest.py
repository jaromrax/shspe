#!/usr/bin/python3
import ROOT
from array import array
LEN=200
# define double array?
x,c = array('d'), array( 'd' )

for i in range( LEN ):
    x.append(i )
    c.append(i+100)

filename="pytest_TGraph.root"    
print("T... create .root file with graph TEST:", filename)
f=ROOT.TFile( filename,"recreate")
gc=ROOT.TGraph( LEN ,x ,c  )
gc.SetName("clona")
gc.SetTitle("clona")
gc.Write()
f.Close()
print("i... graph file created and closed")


filename="pytest_TH1.root"
f=ROOT.TFile( filename,"recreate")
print("T...  file woith TH1 test:  ")
h1=ROOT.TH1F("hpytest","test histogram", 4000,0,4000 )
for i in range(LEN):
    h1.Fill( i )
    if i<100:h1.Fill( i )
    if i<50:h1.Fill( i )

h1.Write()
f.Close()
print("i... TH1F file created and closed")





mmapfile="mmap.histo"
print("T... test mmap feature: (good for gregory)", mmapfile)
allhist=[]
from ROOT import TFile, TH1F, TH1D, TMapFile
f = TMapFile.Create( mmapfile );

print(f)
mr=f.GetFirst()
while f.OrgAddress(mr):
    print(mr,mr.GetClassName() )
    if mr.GetClassName()=="TH1F":
        name=mr.GetName()
    allhist.append( f.Get( name ) )
    #print( mr.GetNext() )
    mr=mr.GetNext()
print("i... printing all histograms:", allhist)
if len(allhist)==0:print("!... no histograms found")
for h in allhist:
    print("h",h)
    h.Draw()
print("i mmap file was tested")
