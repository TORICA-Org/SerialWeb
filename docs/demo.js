window.fetch = async (url, options) => {
  console.log(`Request for: ${url}`);
  let inputData = null;
  let responseOptions = {};
  if (url === '/constantmessage') {
    inputData = 'これはデモです．\n\n世界各国の標準時を表示しています．';
    responseOptions = {
      status: 200,
      headers: {
        'Content-Type': 'text/plain'
      }
    };
  }
  else if (url === '/config') {
    inputData = JSON.stringify({
      'num_of_data': '5',
      'label_0': '日本標準時',
      'label_1': '太平洋標準時',
      'label_2': '中国標準時',
      'label_3': 'グリニッジ標準時',
      'label_4': '湾岸標準時'
    });
    responseOptions = {
      status: 200,
      headers: {
        'Content-Type': 'application/json'
      }
    };
  }
  else if (url === '/data' && options.method === 'POST') {
    let requestBody = '';
    if (options.body instanceof URLSearchParams) {
      requestBody = options.body.get('index');
    }
    else if (typeof options.body === 'string') {
      const params = new URLSearchParams(options.body);
      requestBody = params.get('index');
    }
    const requestIndex = requestBody ? parseInt(requestBody, 10): 0;
    console.log('POST /data received index: ', requestIndex);

    let requestTime;
    const now = new Date();
    switch (requestIndex) {
      case 0:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'Asia/Tokyo'});
        break;
      case 1:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'America/Los_Angeles'});
        break;
      case 2:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'Asia/Shanghai'});
        break;
      case 3:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'Europe/London'});
        break;
      case 4:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'Asia/Dubai'});
        break;
      default:
        requestTime = now.toLocaleTimeString('ja-JP', {timeZone: 'Asia/Tokyo'});
        break;
    }
    inputData = JSON.stringify({
      'index': `${requestIndex}`,
      'content': requestTime
    });
    responseOptions = {
      status: 200,
      headers: {
        'Content-Type': 'application/json'
      }
    };
  }
  else {
    // 未知のURLに対するフェッチリクエストを処理
    console.warn(`Unknown fetch URL: ${url}. Returning 404 Not Found.`);
    inputData = 'Not Found';
    responseOptions = {
      status: 404,
      headers: {
        'Content-Type': 'text/plain'
      }
    };
  }
  return new Response(inputData, responseOptions);
};