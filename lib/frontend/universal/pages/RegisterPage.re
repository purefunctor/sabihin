let registerCss = [%cx
  {|
font-family: "Poppins";

display: flex;
flex-direction: column;
align-items: stretch;

$(Theme.afterMedium) {
  margin-left: auto;
  margin-right: auto;
  max-width: 768px;
}
|}
];

let headingCss = [%cx {|
font-size: 3rem;
text-align: center;
|}];

[@react.component]
let make = () => {
  let username = RegisterPageHooks.useUsername();
  let password = RegisterPageHooks.usePassword(~username=username.value);
  let confirm = RegisterPageHooks.useConfirm(~password=password.value);
  let onSubmit =
    RegisterPageHooks.useFormSubmit(~username, ~password, ~confirm);
  let stage = RegisterPageHooks.useStage();

  <main className=registerCss>
    <h1 className=headingCss> {React.string("Register")} </h1>
    {switch (stage.current) {
     | Form => <RegisterPageForm username password confirm onSubmit />
     | Generate => React.null
     | Submit => React.null
     }}
  </main>;
};
