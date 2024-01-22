const express = require('express');
const bodyParser = require('body-parser');
const app = express();
const port = 3000; 

app.use(bodyParser.urlencoded({ extended: true }));

app.post('/attendance', (req, res) => {
  const cardid = req.body.cardid;
  
  console.log(cardid)
  res.send('success');
});

app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
