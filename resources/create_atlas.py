import subprocess
import os

def gatherImagesList(folder):
    result=set()
    for dp, dn, fn in os.walk(folder):

        dirname=os.path.dirname(dp)
        basename=os.path.basename(dp)

        if dirname != folder or not basename[0].isdigit():
            continue

        print(dp, dirname, basename, dp[-1:].isdigit())

        for f in fn:
            if dp[-1:].isdigit():
                result.add(f)

    for f in sorted(result):
        print("file:", f)

    return sorted(result)

imageTypes=[
    "images/editor_navigation"
]

xOffset=100
currentAtlas=0

for t in imageTypes:
    print("======", t, "======" )

    imageSizes=[]
    imageSizes.extend([16, 20, 24, 28, 32, 40, 48, 56, 64])#, 96, 128])

    fullSize=sum(imageSizes)+len(imageSizes)*4+xOffset
    yStride=max(imageSizes)+5

    allFiles=gatherImagesList(t)

    yPos = 0
    fullCommand = "convert -size {}x{} xc:white -pointsize 14 -fill black".format(fullSize, len(allFiles)*yStride)

    for f in allFiles:
        fullCommand += " -draw \"text 0,{} '{}'\" ".format(yPos+yStride-10, t+"/NNxNN/"+f)

        imageCommand=''
        xPos=0
        for size in imageSizes:
            filename='{}/{}x{}/{}'.format(t, size, size, f)

            if os.path.exists(filename):
                imageCommand+=" -draw \"image over {},{} 0,0 '{}'\"".format(xPos+xOffset, yPos, filename)
            xPos+=size+4

#        print(imageCommand)
        fullCommand+=imageCommand
        yPos += yStride
#        break
#    print(textCommand)

#    fullCommand += textCommand
    fullCommand += " /tmp/atlas_{}_{}.png".format(currentAtlas, t.replace('/', '_'))
    currentAtlas+=1

    print("full command:", fullCommand)

    subprocess.run(fullCommand, shell=True, check=True)
#    break
