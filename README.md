Project Executive Summary

What is the design problem and why it should be solved?

With the growing popularity of student-led solar car racing competitions, newly established teams can have restrictive levels of funding to start their engineering endeavors. One of the most critical components of the competitiveness of their cars is the powertrain efficiency, which is heavily influenced by the quality and versatility of their motor controller. As of today, there are only two competitive options that are adopted by most teams, and both represent a substantial amount of the vehicle's cost (over a third of the electrical system's, approximately). This creates a tall barrier of entry for developing engineers to be exposed to real-life design environments that could deeply affect their career choices and subsequent impact in the world, which is why an open-source, accessible inverter would be impactful in the field of sustainable energy, and potentially many others.

What are the technical problems?

Technical problems include
•	Obtaining efficiency range values comparable or superior to present competitors’ while offering a substantially reduced price: 
Because of the size of the market, no substantial innovation has been made in mainstream options for approximately 14 years. Recent developments in SiC and GaN-based semiconductors could contribute a significant advantage compared to these outdated designs, but hitting required performance levels for a large range of possible motor and voltage configurations will be a challenge. Additionally, new control methodologies might be more applicable nowadays because of both device development and advances in control theory.
•	Maximizing efficiency while keeping the inverter design as tolerant as possible to high-voltage design changes depending on the application: 
Teams' choices of power per motor and DC bus voltage can vary drastically depending on the competition segment (multi- or single-occupant vehicles), creating a challenging optimization problem when attempting to make a product as efficient as possible for as many configurations as expected.
•	Minimizing inverter-driven high voltage system requirements: 
In addition to the DC bus compatibility, motor and motor sensing versatility is a factor that influences how seamless the product's integration can be, facilitating its adoption and potentially expanding its impact.
•	Power density (gravimetric, volumetric):
Being heavily influenced by the switching speed of the semiconductor devices being employed, recent product releases could be incredibly important to meet performance levels such as the DOE's 2025 Vehicle Technology goals.

What is the overall design/technical/objective(s)?

Design a financially (under US$ 1000) and technically accessible, high efficiency inverter for use in solar vehicle applications. The design should be easily replicable, testable, and employed by high-school level students. The final efficiency characteristics for applicable performance ranges (e.g. torque, speed, temperature), should be compatible with or surpass those of available options (WaveSculptor22, Mitsuba).

What is the proof of concept? What demonstrates that the solution works?

The proof of concept for the project could consist of a custom-made PCB for the selected power stage topology, containing components such as the switching devices, gate drivers, & high-voltage sensors. This would be complemented with an off-the-shelf development board containing the selected microcontroller and required peripherals, allowing full validation and operation of the converter without a finalized case and custom logic-stage PCB. Should this implementation work as expected, the project would move forward to unification and/or further customization of the boards to achieve a final unified package that is easily integrable.

Demonstrating the solution may involve attaching either an inductive load or a real motor to the inverter and measuring power losses through the power stage at various operating points (steady state cruising, acceleration, regenerative braking).
