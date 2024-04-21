let registerCss = [%cx {|
font-family: "Poppins";
|}];

let headingCss = [%cx {|
text-align: center;
|}];

let formCss = [%cx {|
display: flex;
flex-direction: column;
gap: 1rem;
|}];

[@react.component]
let make = () => {
  let username = RegisterPageHooks.useUsername();
  let password = RegisterPageHooks.usePassword(~username=username.value);
  let confirm = RegisterPageHooks.useConfirm(~password=password.value);

  <main className=registerCss>
    <h1 className=headingCss> {React.string("Register")} </h1>
    <form className=formCss>
      <RegisterPageFieldUsername hook=username />
      <RegisterPageFieldPassword hook=password />
      <RegisterPageFieldConfirm hook=confirm />
    </form>
  </main>;
};
