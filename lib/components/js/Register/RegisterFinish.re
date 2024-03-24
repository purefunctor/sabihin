[@react.component]
let make = (~formState, ~generateState) => {
  Js.Console.log(formState);
  Js.Console.log(generateState);
  <div> {React.string("Submitting Keys")} </div>;
};
