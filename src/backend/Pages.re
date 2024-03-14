module Base = {
  [@react.component]
  let make = (~children) => {
    <html lang="en">
      <head>
        <meta charSet="UTF-8" />
        <meta
          name="viewport"
          content="width=device-width, initial-scale=1.0"
        />
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
          href="https://fonts.googleapis.com/css2?family=Josefin+Sans:ital,wght@0,100..700;1,100..700&family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap"
          rel="stylesheet"
        />
      </head>
      <body> <div id="root"> children </div> </body>
    </html>;
  };
};

module WithApp = {
  [@react.component]
  let make = (~path) => {
    <Base> <App serverUrl={path, hash: "", search: ""} /> </Base>;
  };

  let render = path => ReactDOM.renderToStaticMarkup(make(~path, ()));
};
