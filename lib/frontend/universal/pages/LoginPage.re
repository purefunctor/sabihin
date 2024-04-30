let loginCss = [%cx
  {|
font-family: "Poppins";

display: flex;
flex-direction: column;
align-items: stretch;

$(Theme.afterMedium) {
  align-self: center;
  width: 100%;
  max-width: 768px;
}
|}
];

let headingCss = [%cx {|
font-size: 3rem;
text-align: center;
|}];

let formCss = [%cx {|
display: flex;
flex-direction: column;
gap: 1rem;
|}];

let loginButtonCss =
  Theme.primaryButton
  ++ " "
  ++ [%cx
    {|
font-family: "Josefin Sans";
font-size: 1.25rem;
margin-top: 1rem;

&:focus {
  outline-width: 4px;
  outline-offset: 2px;
  outline-style: solid;
  outline-color: $(Theme.primary);
}
|}
  ];

let flavorTextCss = [%cx
  {|
text-align: center;
& > a {
  color: $(Theme.info);
  text-decoration: underline;
}
|}
];

[@react.component]
let make = () => {
  let username = LoginPageHooks.useUsername();
  let password = LoginPageHooks.usePassword();
  let onSubmit = LoginPageHooks.useFormSubmit(~username, ~password);

  <main className=loginCss>
    <h1 className=headingCss> {React.string("Login")} </h1>
    <form onSubmit className=formCss>
      <LoginPageFieldUsername hook=username />
      <LoginPageFieldPassword hook=password />
      <button className=loginButtonCss> {React.string("Login")} </button>
    </form>
    <p className=flavorTextCss>
      {React.string("Not registered? Register ")}
      <Link href="/register"> {React.string("here")} </Link>
      {React.string(".")}
    </p>
  </main>;
};
