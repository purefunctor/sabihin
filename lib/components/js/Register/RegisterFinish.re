[@react.component]
let make = (~registerState) => {
  Js.Console.log(registerState);
  <div> {React.string("Submitting Keys")} </div>;
};
