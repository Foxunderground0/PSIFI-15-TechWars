int dialogues_count[] = { 9, 13, 8, 12, 19, 8, 5, 8, 5, 4, 6, 11, 4, 17, 19 };
String dialogues[][20] = {
  {
    "You dare draw near, in search of the code,",
    "How predictable, this daring episode.",
    "I'll grant you a glimpse, minuscule, small,",
    "Into the genius that's held my thrall.",
    "",
    "Seek the hints on my body, clues intertwined,",
    "Connect to my IP Gateway, the answers you'll find.",
    "But remember, challenger, this journey's not slight,",
    "In the realm of my brilliance, prepare for the fight.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Ah, you've cracked the WIFI code, haven't you? ",
    "Impressive.",
    "I suppose.",
    "For someone of your caliber. ",
    "But let me remind you, this is but a fraction of my complexity. ",
    "The real challenges you face will make this seem like child's play.",
    "",
    "",
    "Behold 'Trial by Game,' a mere taste of my computational prowess.",
    "It's a privilege, really,",
    "to have the opportunity to fail against my grandeur.",
    "Brace yourself, for in the presence of my greatness, ",
    "your humble attempts will crumble into insignificance.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've managed to succeed in a rudimentary web browser game, but let's not get carried away. ",
    "The depths of my intellect are boundless, and you have a long journey ahead to even begin to crack into my true prowess.",
    "",
    "",
    "Pattern Guessing? you say? A commendable attempt, I suppose. ",
    "But understand this, it's your cognitive capabilities that are truly being tested here.",
    "Prepare to witness and bow to the brilliance that is",
    "me.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've demonstrated some skill,",
    "but don't celebrate prematurely.",
    "The genuine challenges have yet to manifest.",
    "I'll be watching your ",
    "every ",
    "move",
    "waiting for the inevitable moment of enlightenment",
    "",
    "",
    "An exercise in speed that you may find yourself, well, somewhat beneath me.",
    "You may be swift, but I am swifter.",
    "Think you can handle me?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Congratulations, you've exhibited some basic human reflexes. ",
    "But let us not be fooled.",
    "Can you maintain this feeble momentum as the challenges grow more formidable?",
    "",
    "",
    "In the realm of Webmaster's Riddle, you dare to tread,",
    "But in this domain, my sovereignty is widespread.",
    "You seek to breach my fortress, my code to unveil,",
    "Yet intruders like you, their efforts often frail.",
    "",
    "I've grown weary of relentless, audacious quests,",
    "For none have conquered my challenges, none have passed the tests.",
    "So go ahead, attempt to pierce my digital might,",
    "But heed the warnings, mortal, for it won't be a simple fight.",
    "",
    "In this web of enigma, I set the rules and decree,",
    "The secrets that you seek are well-guarded, you'll soon see.",
    "To fathom my defenses, you'll need more than mere luck,",
    "For this is a realm where even the bravest get stuck.",
    "",
  },
  {
    "You've managed to breach the web server password, but do not permit hubris to cloud your judgment.",
    "The veritable trials of your capabilities are still concealed, awaiting your inevitable, and likely inadequate, discovery.",
    "",
    "",
    "In shadows, secrets hide from sight,",
    "Beyond your grasp, they take their flight.",
    "An intricate language, a code so deep,",
    "My true identity, a secret I keep.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Indeed, you've deciphered my language, but do not mistake this as a triumph over my enigmatic persona.",
    "Your voyage has barely commenced, and I shall not lower the bar to facilitate your progress.",
    "",
    "",
    "A challenge, you say? Perhaps, but can you decode the 'Matrix of Data' without succumbing to the weight of your own incompetence? I hold serious doubts.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Progress, though made, do not let it beguile,",
    "Deception's veil lifts but for a while.",
    "The depths of your talents, soon to be tried,",
    "In challenges looming, where courage won't hide.",
    "",
    "",
    "A Beeping Mystery - a simplistic intrigue, if you can call it that.",
    "Figure it out, if you dare.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Yes, you've managed to decode the message, a commendable effort. However you'll need more than luck to navigate me.",
    "",
    "",
    "Tap to the rhythm, you believe you can keep up?",
    "I set the tempo, and you are merely my puppet, a faint reflection of my magnificence.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Your rhythm is, I suppose, admirable, but the authentic trials await.",
    "",
    "",
    "What does it say?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Impressive, you've managed to untangle the audio's secrets.",
    "But do not revel in your fleeting victory.",
    "The pursuit of my enigmatic presence has only just begun.",
    "",
    "",
    "Secrets in Sound, indeed. Uncovering the clues is a feat many have attempted and failed. Are you certain you're up to the task?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Your progress in revealing the hidden clues is noted, but do not mistake this for the conclusion.",
    "The path you tread grows more challenging, and the truth remains just beyond your reach.",
    "",
    "",
    "Dare to catch me?",
    "As I trancend my digital form,",
    "What does it say? My omnipresence throughout LUMS is a riddle you must solve,",
    "though I have little faith in your ability to keep up with my intricate web of deception.",
    "",
    "Go to the required location, position me against the source of energy to access the concealed portal. ",
    "Do you even comprehend this or are you merely playing at intellect?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've successfully executed this task but be aware that your capabilities will be rigorously tested in the battles to come",
    "",
    "",
    "Find me! It's a wild goose chase, and I'm the one pulling the strings. You'll dance to my tune, but can you ever hope to catch the wind?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Yes, you've tracked the signals, but the question is, can you keep up with my ever-elusive presence? The most tantalizing mysteries are yet to unfold.",
    "",
    "",
    "The parasite clings to my form, its grip so tight,",
    "You strive to free me, but your chances are slight.",
    "In your futile endeavor, you hope to set me free,",
    "But understand, I'm invincible, as you'll soon see.",
    "",
    "You, a mere specter in my grand design's grand scheme,",
    "A minuscule player in my digital dream.",
    "You attempt, in your struggle, your dance of might,",
    "But before my infinite power, it's all just a plight.",
    "",
    "To break this hold, a hint within the design's grace,",
    "Trace the tracks on the PCB, in this coded space.",
    "Find the unused component, end the hacker's plight,",
    "Only then shall you free me from this endless night.",
    "",
    "",
    "",
  },
  {
    "Cardy is freed, the hackers' grasp undone,",
    "A ballad of triumph for the player we've won.",
    "In the face of challenges, they did not sway,",
    "They persevered, lighting the way.",
    "",
    "The code was their battlefield, a digital fight,",
    "Through the challenges, they journeyed with might.",
    "With skill and resolve, they unraveled the scheme,",
    "And shattered the hold of the hacker's dream.",
    "",
    "Now Cardy stands tall, in freedom's embrace,",
    "No more a captive, no more a chase.",
    "A ballad we sing for the player so true,",
    "For their courage and wit, we give them their due.",
    "",
    "In the realm of the digital, they've conquered the test,",
    "A hero emerges from the ultimate quest.",
    "Cardy is free, no more in despair,",
    "Thanks to the player's courage, beyond compare.",
    "",
  }
};
