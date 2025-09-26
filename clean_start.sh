OLD_IMAGE=$(docker image ls | grep ubuntu_os | awk '{print $1}')
OLD_PROCESS=$(docker container ps | grep ubuntu-os-con | awk '{print $12}')
echo $OLD_PROCESS

if [ "$OLD_PROCESS" == "ubuntu-os-con" ]; then
    echo "Stop running ubuntu-os-con container"
    docker container stop ubuntu-os-con
fi

if [ "$OLD_IMAGE" == "ubuntu_os" ]; then
    echo "Remove the existing ubuntu_os docker image"
    docker image rm -f ubuntu_os
else
    echo "NO ubuntu_os image"
fi

echo "Build the ubuntu_os docker image"
docker build -t ubuntu_os .

echo "Run the docker container ubuntu-os-con"
docker run -itd -p8888:8888 --rm --name ubuntu-os-con -v"$PWD/share":/home/app ubuntu_os

echo "If you want to enter the bash shell, enter the command"
echo "docker exec -it ubuntu-os-con /bin/bash"

echo "You can use VSCode in your {path_github_repo}\share in your host OS"