[@react.component]
let make = (~registerState, ~generateState) => {
  Js.Console.log(registerState);
  Js.Console.log(generateState);
  <div> {React.string("Submitting Keys")} </div>;
};
