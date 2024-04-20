module Template = {
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
        <link rel="stylesheet" href="assets/global.css" />
        <style> {React.string(CssJs.render_style_tag())} </style>
      </head>
      <body> <div id="root"> children </div> </body>
    </html>;
  };
};

let toString = path => {
  let serverUrl: ReasonReactRouter.url = {path, hash: "", search: ""};
  "<!doctype html>"
  ++ ReactDOM.renderToString(
       <Template>
         <ServerUrl.Provider value={Some(serverUrl)}>
           <App />
         </ServerUrl.Provider>
       </Template>,
     );
};
