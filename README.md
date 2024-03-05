Requisitos:
WSL2, Docker Desktop y Minikube.


Para instalar WSL2 puedes seguir este tutorial https://www.linkedin.com/pulse/step-procedure-install-wsl2-windows-run-ubuntu-using-arun-kl/


Para instalar Docker Desktop lo puedes encontrar aquí https://docs.docker.com/desktop/install/windows-install/

Para instalar Minikube y Kubectl correremos los siguientes comandos en la consola de WSL2:

```curl -Lo minikube https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64```

```chmod +x ./minikube```

```sudo mv ./minikube /usr/local/bin/```

```minikube config set driver docker```

```curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"``` 

```sudo install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl```

Pasos para ejecutar ptrace_example:
1. Clonamos el repositorio usando: ```git clone https://github.com/eloygaroz/TFM```
2. Accedemos al repositorio clonado: ```cd /TFM```
3. Una vez en el repo, buildeamos la imagen usada para el deployment, ebpf_test:1.0 a través de Docker Desktop:
   
   ```sudo docker build -t ebpf_test:1.0 .```

   Si al correr el comando de arriba, sale este error:

   **ERROR: failed to solve: ubuntu:20.04: error getting credentials - err: fork/exec /usr/bin/docker-credential-desktop.exe: exec format error, out: ``**

   Deberemos de cambiar el archivo de ./docker/config.json de **credsStore** a **credStore**
   

4. Cargamos la imagen en minikube: ```minikube image load ebpf_test:1.0```

Si nos diera el error de **Exiting due to GUEST_IMAGE_LOAD** usaremos el siguiente workaround:

```docker image save -o ebpf_test.tar ebpf_test:1.0```

```minikube image load ebpf_test.tar```
   
5. Realizamos el deployment usando el archivo .yaml: ```kubectl apply -f sidecar_deployment.yaml```
6. Para comprobar que el deployment se ha realizado correctamente ejecutamos: ```kubectl get pods```. Deberíamos de obtener el resultado de abajo

      ![image](https://github.com/eloygaroz/TFM/assets/62937614/37977fb5-7fd0-48ab-886a-d4139d509b8b)
  
7. Abrimos la consola del sidecar ejecutando ```kubectl exec -it sidecar-bpf-deployment-5c87d954c-8twj6 -c ebpf-container -- /bin/sh```

    El código que aparece después de **sidecar-bpf-deployment**, en este caso **5c87d954c-8twj6**, deberá ser sustituido por el que tengamos.

8. En esta consola, ejecutamos ```./ecc signal.c signal.h``` para compilar el programa y ```./ecli package.json``` para correrlo.

9. Abrimos la consola del contenedor principal ejecutando ```kubectl exec -it sidecar-bpf-deployment-5c87d954c-8twj6 -- /bin/sh``` en otra consola de wsl diferente.

De nuevo, sustituyendo **5c87d954c-8twj6** por el código que tengamos.

10. Una vez en la consola del contenedor principal, corremo ```apt-get update``` y ```apt-get install strace```
11. Finalmente, ejecutaremos ```strace /bin/whoami``` en la consola del contenedor principal, y debería de aparecer el mensaje **Killed** en pantalla, y aparecer un nuevo log en la consola del sidecar.
