[@react.component]
let make = (~children) => {
  <html lang="en">
    <head>
      <meta charSet="UTF-8" />
      <meta name="viewport" content="width=device-width, initial-scale=1.0" />
      <title> {React.string("Sabihin")} </title>
      <script type_="module" src="static/index.js" />
      <link rel="stylesheet" href="assets/index.css" />
      <link rel="preconnect" href="https://fonts.googleapis.com" />
      <link
        rel="preconnect"
        href="https://fonts.gstatic.com"
        crossOrigin="anonymous"
      />
      <link
        href="https://fonts.googleapis.com/css2?family=Josefin+Sans:ital,wght@0,100..700;1,100..700&display=swap"
        rel="stylesheet"
      />
    </head>
    <body> <div id="root"> children </div> </body>
  </html>;
};
