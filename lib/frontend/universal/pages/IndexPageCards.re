module IndexCard = {
  let featureCardCss = [%cx
    {|
    background-color: $(Theme.backgroundSubtle);
    border-radius: 8px;

    display: flex;
    justify-content: space-between;
    gap: 1rem;
    padding: 1rem;
  |}
  ];

  let articleCss = [%cx {| font-family: "Poppins" |}];

  let headingCss = [%cx {|
    margin: 0 0 0.5rem 0;
  |}];

  let iconCss = [%cx {|
    color: $(Theme.primary);
    flex-shrink: 0;
  |}];

  let pCss = [%cx
    {|
    color: $(Theme.foregroundSubtle);
    margin-top: 0.5rem;
    margin-bottom: 0.5rem;
    &:last-child {
      margin-bottom: 0;
    }
  |}
  ];

  [@react.component]
  let make = (~title, ~iconFn, ~children) => {
    <div className=featureCardCss>
      <article className=articleCss>
        <h1 className=headingCss> {React.string(title)} </h1>
        children
      </article>
      {iconFn(~size="3rem", ~className=iconCss)}
    </div>;
  };
};

let cardsCss = [%cx
  {|
display: flex;
flex-direction: column;
gap: 2rem;

$(Theme.afterMedium) {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
}
|}
];

[@react.component]
let make = () => {
  <section className=cardsCss>
    <IndexCard
      title="End-to-End Encryption"
      iconFn={(~size, ~className) => <Icons.MailLockLine size className />}>
      <p className=IndexCard.pCss>
        {React.string("Sabihin utilizes end-to-end encryption techniques.")}
      </p>
      <p className=IndexCard.pCss>
        {React.string("Messages can only be read by its intended recipients.")}
      </p>
    </IndexCard>
    <IndexCard
      title="Open-Source Software"
      iconFn={(~size, ~className) => <Icons.RepositoryLine size className />}>
      <p className=IndexCard.pCss>
        {React.string(
           "Sabihin is built with open-source tools and is open-source itself.",
         )}
      </p>
      <p className=IndexCard.pCss>
        {React.string(
           "Its source code is public and contributions are welcome!",
         )}
      </p>
    </IndexCard>
    <IndexCard
      title="Functional Programming"
      iconFn={(~size, ~className) => <Icons.OCamlFileIcon size className />}>
      <p className=IndexCard.pCss>
        {React.string(
           "Sabihin is built with functional programming languages.",
         )}
      </p>
      <p className=IndexCard.pCss>
        {React.string(
           "OCaml offers performance, reliability, and fast iteration.",
         )}
      </p>
    </IndexCard>
  </section>;
};
