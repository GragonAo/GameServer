
args=$1

clean=false


if [ ${args}v = "clean"v ];then
    clean=true
fi

build(){
    for i in `ls -d */`;do
        cd $i/Build
        if ${clean};then
            rm -rf *
        else
            cmake ..
            make -j4
        fi
        cd ../..
    done
}

cd src/libs/
build

cd ../apps/
build

cd ../tools/
build