open WritePageHooks;
open WritePageStyles;

[@react.component]
let make = () => {
  let (_, remaining, setMessage) = useMessage();

  <main className=writeCss>
    <h1> {React.string("Compose")} </h1>
    <form className=formCss>
      <div className=fieldCss>
        <label htmlFor="recipient"> {React.string("Recipient")} </label>
        <input id="recipient" name="recipient" className=recipientInputCss />
      </div>
      <div className=fieldCss>
        <span id="message-label"> {React.string("Message")} </span>
        <WritePageEditor ariaLabelledBy="message-label" setMessage />
        <span className=messageRemainingCss>
          {remaining |> string_of_int |> React.string}
        </span>
      </div>
      <div className=actionGroupCss>
        <button className=clearCss>
          <Icons.CloseLine size="1.2rem" />
          {React.string("Clear")}
        </button>
        <button className=sendCss>
          <Icons.MailSendLine size="1.2rem" />
          {React.string("Send")}
        </button>
      </div>
    </form>
  </main>;
};
