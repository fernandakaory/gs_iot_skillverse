# 📊 Monitor de Frequência Cardíaca com ESP32 e MQTT

## 🎯 Global Solutions 2025 - O Futuro do Trabalho

### 👥 Equipe
- Anny Carolina Andrade Dias | RM98295 </br>
- Fernanda Kaory Saito | RM551104 </br>
- Pedro Emerici Gava | RM551043

---

## 📋 Descrição do Projeto
> Link do vídeo no Youtube: https://youtu.be/A25w7y42Se4
### Problema Abordado

No contexto do **futuro do trabalho**, profissionais enfrentam desafios crescentes relacionados ao **estresse, sobrecarga** e **problemas de saúde** causados por rotinas intensas, especialmente em ambientes de home office e trabalho híbrido. A ausência de monitoramento da saúde em tempo real pode levar a:

- Fadiga crônica e burnout
- Problemas cardiovasculares
- Redução da produtividade
- Afastamentos médicos

### Solução Proposta

Desenvolvemos um **sistema IoT de monitoramento de frequência cardíaca** que:

1. **Monitora em tempo real** a frequência cardíaca do profissional
2. **Detecta automaticamente** quando a frequência está elevada (acima de 120 BPM)
3. **Envia alertas via MQTT** sugerindo pausas inteligentes
4. **Promove bem-estar** no ambiente de trabalho através de tecnologia preventiva

Este projeto se alinha ao tema **"Saúde e bem-estar no trabalho"** proposto pela Global Solutions, utilizando:
- **ESP32** como microcontrolador principal
- **Potenciômetro** simulando sensor de frequência cardíaca (pode ser substituído por sensor real MAX30100/MAX30102)
- **LEDs** para feedback visual (verde=normal, amarelo=moderado, vermelho=alto)
- **Protocolo MQTT** para comunicação IoT em tempo real
- **Simulação no Wokwi** para demonstração prática

---

## 🔧 Componentes Utilizados

### Hardware
- **1x ESP32 DevKit V1** - Microcontrolador com WiFi integrado
- **1x Potenciômetro 10kΩ** - Simula sensor de frequência cardíaca
- **3x LEDs** (Verde, Amarelo, Vermelho) - Indicadores visuais de status
- **3x Resistores 220Ω** - Limitadores de corrente para LEDs
- **Protoboard e jumpers** - Conexões

### Software
- **Arduino IDE** (versão 1.8.7 ou superior)
- **Biblioteca PubSubClient** (para MQTT)
- **Biblioteca WiFi** (nativa do ESP32)
- **Broker MQTT** (HiveMQ público ou Mosquitto local)

---

## 📐 Diagrama do Circuito

### Conexões:

**Potenciômetro (Sensor de Frequência Cardíaca):**
- Pino 1 (extremo) → 3.3V do ESP32
- Pino 2 (central) → GPIO34 do ESP32
- Pino 3 (extremo) → GND do ESP32

**LEDs:**
- **LED Verde** → GPIO26 + Resistor 220Ω → GND
- **LED Amarelo** → GPIO27 + Resistor 220Ω → GND
- **LED Vermelho** → GPIO25 + Resistor 220Ω → GND

### Link do Projeto no Wokwi
🔗 [Link para simulação no Wokwi](https://wokwi.com/projects/381957492268331009)

---

## 🌐 Configuração MQTT

### Tópicos Utilizados

| Tópico | Tipo | Descrição |
|--------|------|-----------|
| `fiap/gs/heartrate/bpm` | Publish | Publica o valor de BPM lido |
| `fiap/gs/heartrate/status` | Publish | Publica o status (NORMAL/MODERADA/ALTA) |
| `fiap/gs/heartrate/alert` | Publish | Publica alertas quando BPM > 120 |

## Broker MQTT

### Monitoramento via Web Client (HiveMQ WebSocket)

Para visualizar os dados MQTT em tempo real através do navegador, use o cliente web do HiveMQ:

**Acesse:** https://www.hivemq.com/demos/websocket-client/

**Configurações de Conexão:**
- **Host:** `broker.hivemq.com`
- **Port:** `8884` (WebSocket + SSL)
- **Username:** deixe em branco (ou use credenciais se tiver conta HiveMQ Cloud)
- **Password:** deixe em branco
- **SSL:** ✅ **MARQUE esta opção** (obrigatório para porta 8884)
- **ClientID:** qualquer nome único (ex: `monitor-heartrate-123`)

**Após conectar:**
1. No painel "Subscriptions", clique em "Add New Topic Subscription"
2. Digite: `fiap/gs/heartrate/#`
3. Você verá todas as mensagens publicadas pelo ESP32 em tempo real!

**Observação:** A porta 8884 é para WebSocket com SSL (navegador). O ESP32 continua usando a porta 1883 (MQTT padrão sem SSL).

---

## 🚀 Como Usar

### Passo 1: Preparar o Ambiente

1. **Instale o Arduino IDE:**
   - Download: https://www.arduino.cc/en/software

2. **Configure o ESP32 no Arduino IDE:**
   - Vá em `Arquivo > Preferências`
   - Em "URLs Adicionais para Gerenciadores de Placas", adicione:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Vá em `Ferramentas > Placa > Gerenciador de Placas`
   - Busque por "ESP32" e instale

3. **Instale a Biblioteca PubSubClient:**
   - Vá em `Sketch > Incluir Biblioteca > Gerenciar Bibliotecas`
   - Busque por "PubSubClient" (by Nick O'Leary)
   - Clique em "Instalar"

### Passo 2: Configurar o Código

1. **Abra o arquivo `heart_rate_monitor.ino`**

2. **Configure suas credenciais WiFi:**
```cpp
const char* ssid = "SEU_WIFI";          // Nome da sua rede WiFi
const char* password = "SUA_SENHA";     // Senha da sua rede WiFi
```

3. **Configure o broker MQTT (opcional):**
```cpp
const char* mqtt_server = "broker.hivemq.com";  // Ou seu broker local
```

### Passo 3: Upload e Teste

1. **Conecte o ESP32 ao computador** via USB
2. **Selecione a placa:** `Ferramentas > Placa > ESP32 Dev Module`
3. **Selecione a porta:** `Ferramentas > Porta > COM[X]` (Windows) ou `/dev/ttyUSB[X]` (Linux)
4. **Faça o upload:** Clique no botão de upload (→)
5. **Abra o Serial Monitor:** `Ferramentas > Monitor Serial` (115200 baud)

### Passo 4: Monitorar via MQTT

Para visualizar as mensagens MQTT em tempo real, use um cliente MQTT:

**Opção 1: HiveMQ Web Client (Navegador)**
- Acesse: https://www.hivemq.com/demos/websocket-client/
- Configure:
  - Host: `broker.hivemq.com`
  - Port: `8884`
  - SSL: ✅ **Marque esta opção**
- Após conectar, assine o tópico: `fiap/gs/heartrate/#`
- Visualize os dados em tempo real!



# 📊 Funcionamento do Sistema

### Lógica de Monitoramento

O sistema opera com os seguintes limites:

| Status | Faixa BPM | LED | Ação |
|--------|-----------|-----|------|
| **Normal** | 40-99 BPM | 🟢 Verde | Monitoramento contínuo |
| **Moderado** | 100-119 BPM | 🟡 Amarelo | Alerta de atenção |
| **Alto** | ≥120 BPM | 🔴 Vermelho | **ALERTA: Fazer pausa!** |

### Fluxo de Operação

```
1. ESP32 lê valor do potenciômetro (0-4095)
   ↓
2. Converte para BPM (40-180)
   ↓
3. Avalia o status (Normal/Moderado/Alto)
   ↓
4. Atualiza LED correspondente
   ↓
5. Publica dados via MQTT:
   - Valor de BPM
   - Status atual
   - Alerta (se BPM ≥ 120)
   ↓
6. Aguarda 2 segundos e repete
```

### Exemplo de Alerta

Quando a frequência cardíaca atinge **120 BPM ou mais**, o sistema:

1. ✅ Acende o **LED vermelho**
2. 📡 Publica no tópico `fiap/gs/heartrate/alert`:
   ```
   ATENCAO! Frequencia cardiaca elevada: 125 BPM. Faca uma pausa!
   ```
3. 📺 Exibe no Serial Monitor:
   ```
   ⚠️  ALERTA: Frequência cardíaca alta!
   💡 Sugestão: Faça uma pausa e relaxe!
   ```

---

## 🔄 Possíveis Melhorias Futuras

1. **Sensor Real:** Substituir potenciômetro por sensor MAX30100/MAX30102
2. **Dashboard Web:** Criar interface web para visualização em tempo real
3. **Integração com Apps:** Enviar notificações para smartphone
4. **Machine Learning:** Detectar padrões de estresse personalizados
5. **Histórico de Dados:** Armazenar dados em banco de dados para análise
6. **Múltiplos Usuários:** Sistema multi-usuário para empresas

---

## 📚 Dependências

### Bibliotecas Arduino
```cpp
#include <WiFi.h>          // Nativa do ESP32
#include <PubSubClient.h>  // Instalar via Library Manager
```

### Ferramentas Externas
- **Arduino IDE** 1.8.7+
- **ESP32 Board Package** 2.0.0+
- **Cliente MQTT** (MQTT Explorer, HiveMQ Web Client, Mosquitto, etc.)


## 💡 Impacto no Futuro do Trabalho

### Benefícios Diretos

✅ **Prevenção de Problemas de Saúde:** Detecção precoce de estresse cardiovascular  
✅ **Aumento da Produtividade:** Pausas estratégicas baseadas em dados reais  
✅ **Bem-estar do Trabalhador:** Cuidado com a saúde mental e física  
✅ **Redução de Afastamentos:** Prevenção de burnout e problemas cardíacos  
✅ **Cultura de Saúde:** Promove conscientização sobre autocuidado  

### Aplicações Práticas

- **Empresas de Tecnologia:** Monitoramento de desenvolvedores em sprints intensos
- **Call Centers:** Acompanhamento de operadores sob pressão
- **Home Office:** Gestão pessoal de saúde durante trabalho remoto
- **Hospitais:** Monitoramento de profissionais de saúde em plantões
- **Educação:** Professores e estudantes em períodos de avaliação

---

## 📖 Referências

- [ESP32 ADC Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
- [PubSubClient Library](https://pubsubclient.knolleary.net/)
- [MQTT Protocol](https://mqtt.org/)
- [HiveMQ Public Broker](https://www.hivemq.com/mqtt/public-mqtt-broker/)
- [HiveMQ WebSocket Client](https://www.hivemq.com/demos/websocket-client/)
- [Arduino ESP32 Guide](https://randomnerdtutorials.com/getting-started-with-esp32/)

---

## 📝 Licença

Este projeto foi desenvolvido para fins educacionais como parte da **Global Solutions 2025 - FIAP**.

---


**⚡ Desenvolvido com ESP32 e dedicação para um futuro do trabalho mais saudável! ⚡**
