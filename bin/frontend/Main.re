switch (ReactDOM.querySelector("#root")) {
| Some(element) =>
  let _ = ReactDOM.Client.hydrateRoot(element, <App />);
  ();
| None => ()
};
