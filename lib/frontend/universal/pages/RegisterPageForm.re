let formCss = [%cx {|
display: flex;
flex-direction: column;
gap: 1rem;
|}];

let registerButtonCss =
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
let make = (~username, ~password, ~confirm, ~onSubmit) => {
  <>
    <form onSubmit className=formCss>
      <RegisterPageFieldUsername hook=username />
      <RegisterPageFieldPassword hook=password />
      <RegisterPageFieldConfirm hook=confirm />
      <button className=registerButtonCss>
        {React.string("Register")}
      </button>
    </form>
    <p className=flavorTextCss>
      {React.string("Already registered? Log in ")}
      <Link href="/login"> {React.string("here")} </Link>
      {React.string(".")}
    </p>
  </>;
};
