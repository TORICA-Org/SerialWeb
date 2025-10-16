// WebページのHTML

#ifndef INDEX_H
#define INDEX_H

const char html[] = R"(

<!DOCTYPE html>
<html lang='ja'>

<head>
  <meta charset='utf-8' />
  <meta name='viewport' content='width=device-width,initial-scale=1' />
  <title>TORICA WebServer</title>
</head>
<style>
  body {
    font-family: arial;
    background-color: white;
    text-align: center;
  }
  svg {
    margin: 30px 0;
  }
  form {
    display: flex;
    justify-content: center;
    align-items: center;
    gap: 10px;
    width: 100%;
    margin: 0 auto;
  }
  input {
    display: block;
    height: fit-content;
    flex-grow: 1;
    flex-basis: 0;
    min-width: 0;
  }
  #formWrapper {
    margin: 20px auto;
    width: 90%;
    max-width: 500px;
    display: none;
  }
  #constantMessage {
    margin: 10px auto;
    padding: 1rem;
    box-sizing: border-box;
    width: 90%;
    max-width: 500px;
    font-size: large;
    text-align: left;
    word-wrap: break-word;
    white-space: pre-line;
    /* 改行コードを有効化 */
    border: solid 1px black;
    border-radius: 5px;
  }
  #dataWrapper {
    margin: 10px auto;
    width: 90%;
    max-width: 500px;
  }
  .dataBlock {
    display: flex;
    gap: 1rem;
    align-items: flex-end;
    margin: 10px auto;
    background-color: rgba(0, 0, 0, 0.05);
    border: solid 1px black;
    border-radius: 5px;
  }
  .label {
    display: block;
    font-size: x-large;
    font-weight: bold;
    flex-grow: 1;
    flex-basis: 0;
    text-align: center;
  }
  .content {
    display: block;
    font-size: x-large;
    font-weight: bold;
    flex-grow: 1;
    flex-basis: 0;
    text-align: center;
  }
</style>

<body>
  <svg width='350' height='150' xmlns='http://www.w3.org/2000/svg'>
    <g style='display:inline'>
      <path style='fill:#000;fill-opacity:1;stroke:#000;stroke-width:.475027;stroke-opacity:1'd='M434 252s48 12 107 35c12 5 40 18 31 29-8 10-30 14-48 16l9-9c3-3 6-6 8-9 2-4-1-5-4-3l-3 1c-1 1-3 2-5 3l-4-16-3-6h-10l-15 32h10l2-8h13c0 4-3 5-5 6-5 4-11 7-17 11-29 3-60 3-111 2-31 0-73-2-107-4 1-5 1-11 1-11s-9 4-13 11c-11-1-30-2-30-2s13 1 29 2c-3 7-11 31-9 46 1 8 6 17 16 16 11-1 4-13 8-19 2-2 10 0 13 0 1 3 7 2 10 2 10 1 20 0 29 2 7 1 13 3 20 3 4 0 10-2 13 0 5 2 9 6 14 8s10 3 15 5c2 2 3 6 6 6s7-2 10-2c9 1 18 1 27 1 4 0 8 1 12-1 3-1 0-4-2-5-4-4-8-9-14-10 0-3 3-3 6-4 6-1 12-3 17-5 18-6 33-18 47-30 5-3 9-7 14-11 25-3 52-8 59-19 13-19-19-30-51-39-66-20-95-24-95-24zm-200 11v12l28 2-3 31v10h22l2-38 27 2v-12zm110 11c-1 0-3 0-5 1-8 1-15 3-21 8-7 6-7 19-1 27 10 11 24 13 38 11 16-2 32-19 20-35-7-8-19-12-31-11zm44 6-2 42h14v-16c11 0 19 5 19 16l13 1c0-6-5-16-11-19v-1c2-1 6-1 8-3 5-6 0-14-6-16-10-5-24-3-35-4zm-42 4c6 0 12 1 16 6 6 9-2 21-12 23-6 0-14-2-17-6-6-9 0-22 10-23h3zm95 1-2 37 11 1 3-38zm36 3c-9 0-17 4-19 14-1 6 1 13 5 18 10 9 27 6 32-7l-10-1c-1 6-10 7-14 3-7-6-3-22 8-20 4 0 5 4 7 5 2 2 6 1 9 1 0-7-8-11-14-12h-4zm-75 1h7c2 0 4 1 5 1 4 1 6 4 3 7-2 2-13 2-15 0-1-2 0-6 0-8zm116 10 2 12h-8zm-236 34c30 2 69 4 100 5 50 0 94 0 115-1-5 3-10 7-14 9-10 7-20 13-30 19-6 3-13 9-20 11-4 1-9 3-12 3s-6-2-8-2c-6-1-14-2-20-2-4 0-8-1-12 0-2 0-2 2-5 2-5 0-10-2-15-3-15-4-28-7-41-15-6-4-13-7-18-13-2-2-3-6-6-8 1 14 12 22 22 29 5 2 11 4 14 8-5-2-18 2-21-2-3 0-5 0-7-2 4 0 10 0 14-2v-1h-24c-2 3-11 3-12-1-2-11 0-29 0-34z'transform='translate(-233.5 -251.506)' />
    </g>
  </svg>
  <p id='constantMessage'></p>
  <div id='formWrapper'>
    <form id='form' method='POST' action=''>
      <input type='number' id='commandId' placeholder="ID" />
      <input type='text' id='commandContent' placeholder="COMMAND" />
      <button type='submit'>SUBMIT</button>
    </form>
  </div>
  <div id='dataWrapper'></div>

  <script type='text/javascript'>
    let numOfData;
    (async function main () {
      const constantMessage = await getConstantMessage();
      document.getElementById('constantMessage').textContent = constantMessage;
      await new Promise(resolve => setTimeout(resolve, 300));

      const config = await getConfig();
      numOfData = parseInt(config.num_of_data, 10);

      for (let i = 0; i < numOfData; i++) {
        const newDataBlock = document.createElement('div');
        newDataBlock.classList.add('dataBlock');
        newDataBlock.id = `data_${i}`;

        const newLabel = document.createElement('p');
        newLabel.classList.add('label');
        newLabel.textContent = config[`label_${i}`];

        const newContent = document.createElement('p');
        newContent.classList.add('content');
        newContent.id = `content_${i}`;

        newDataBlock.appendChild(newLabel);
        newDataBlock.appendChild(newContent);

        document.getElementById('dataWrapper').appendChild(newDataBlock);
      }
      await new Promise(resolve => setTimeout(resolve, 300));

      updateContent();
    })();

    async function getConstantMessage () {
      try {
        const response = await fetch('/constantmessage');
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const responseMessage = await response.text();
        return responseMessage;
      }
      catch (error) {
        console.error('getConstantMessage failed: ', error);
        await new Promise(resolve => setTimeout(resolve, 300));
        return getConstantMessage();
      }
    }

    async function getConfig () {
      try {
        const response = await fetch('/config');
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const responseConfig = await response.json();
        return responseConfig;
      }
      catch (error) {
        console.error('getConfig failed: ', error);
        await new Promise(resolve => setTimeout(resolve, 300));
        return getConfig();
      }
    }

    let requestIndex = 0;
    async function updateContent() {
      try {
        const formData = new URLSearchParams();
        formData.append('index', requestIndex.toString());

        const options = {
          method: 'POST',
          headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
          },
          body: formData
        }
        const response = await fetch('/data', options);
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const responseContent = await response.json(); 
        const nowUpdate = document.getElementById(`content_${responseContent.index}`);
        if (nowUpdate) {
          nowUpdate.textContent = responseContent.content;
        }
        else {
          console.warn(`${responseContent.index} not found.`);
        }
        requestIndex++;
        if (requestIndex >= numOfData) {
          requestIndex = 0;
        }
      }
      catch (error) {
        console.error('updateContent failed: ', error);
      }
      finally {
        setTimeout(() => {
          updateContent();
        }, 100);
      }
    }
  </script>
</body>
</html>

)";

#endif // INDEX_H
