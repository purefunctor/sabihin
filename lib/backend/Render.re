module Page = {
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
        <script type_="module" src="http://localhost:5173/@vite/client" />
        <script type_="module" src="http://localhost:5173/index.js" />
      </head>
      <body> <div id="root"> children </div> </body>
    </html>;
  };
};

module AppPage = {
  [@react.component]
  let make = () => {
    <Page> <App /> </Page>;
  };

  let render = _ => ReactDOM.renderToStaticMarkup(make());
};
