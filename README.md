# PGCCHIB - Processamento Gráfico

Este repositório é baseado na estrutura do projeto PGCCHIB - Processamento Gráfico, fornecido pela disciplina de Processamento Gráfico.

A atividade foi implementada no arquivo `jogoDasCores.cpp`, localizado em `src/Vivencial`.

https://youtu.be/JfuY5sxkjcE

![image](https://github.com/user-attachments/assets/3a8fbaf0-bbff-403b-b835-792a9bc3c7d9)

![image](https://github.com/user-attachments/assets/827ef6b5-62ff-4139-b6cf-69ea92d434a5)

![image](https://github.com/user-attachments/assets/6993d4e5-a246-4435-8b9b-b04bca3a35fd)

## 🛠️ Como compilar e rodar o projeto (PowerShell - Windows)
Se quiser compilar e rodar o projeto do zero, siga os comandos abaixo:
```powershell
cd ~seudiretório\V1_ProcessamentoGrafico\
Remove-Item -Recurse -Force .\build\
mkdir build
cd build
cmake ..
cmake --build .
.\Debug\Triangulos.exe
```

Siga as instruções detalhadas em [GettingStarted.md](GettingStarted.md) para configurar e compilar o projeto.

## ⚠️ **IMPORTANTE: Baixar a GLAD Manualmente**
Para que o projeto funcione corretamente, é necessário **baixar a GLAD manualmente** utilizando o **GLAD Generator**.

### 🔗 **Acesse o web service do GLAD**:
👉 [GLAD Generator](https://glad.dav1d.de/)

### ⚙️ **Configuração necessária:**
- **API:** OpenGL  
- **Version:** 3.3+ (ou superior compatível com sua máquina)  
- **Profile:** Core  
- **Language:** C/C++  

### 📥 **Baixe e extraia os arquivos:**
Após a geração, extraia os arquivos baixados e coloque-os nos diretórios correspondentes:
- Copie **glad.h** para `include/glad/`
- Copie **khrplatform.h** para `include/glad/KHR/`
- Copie **glad.c** para `common/`

🚨 **Sem esses arquivos, a compilação falhará!** É necessário colocá-los nos diretórios corretos conforme a orientação acima.

---



